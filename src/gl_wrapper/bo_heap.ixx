module;

#include "util/gl.h"

export module bo_heap;

import <memory>;
import <vector>;
import <list>;

import glm;
import vertex_layout;
import buffer_object;

import debug;

struct HeapBufferObject;

export struct BufferObjectHeapHandle {
  HeapBufferObject *parent = nullptr;
  GLuint offset = 0, size = 0;

  BufferObjectHeapHandle() = default;
  BufferObjectHeapHandle(HeapBufferObject *parent, const GLuint offset,
                         const GLuint size);
  ~BufferObjectHeapHandle();
};
struct HeapBufferObject : GL::BufferObject {
  static constexpr auto MAX_SIZE = 0xffffffu;

  struct FreeBlock {
    GLuint offset, size;
  };
  using free_list = std::list<FreeBlock>;
  free_list freeList{{0, MAX_SIZE}};

  HeapBufferObject();
  ~HeapBufferObject() = default;
  HeapBufferObject(const HeapBufferObject &) = delete;
  HeapBufferObject(HeapBufferObject &&o);
  HeapBufferObject &operator=(const HeapBufferObject &) = delete;
  HeapBufferObject &operator=(HeapBufferObject &&o);

  void free(const BufferObjectHeapHandle *handle);

private:
  void coalesce(const free_list::iterator &block);
  void coalesceRight(const free_list::iterator &block);
};

export struct VBOHeapHandleSubData : BufferObjectHeapHandle {
  GLuint count = 0, vertexSize = 0;

  VBOHeapHandleSubData() = default;
  VBOHeapHandleSubData(HeapBufferObject *parent, const GLuint offset,
                       const GLuint size, const GLuint vertexSize);

  void writeRaw(const void *data, const GLuint size, const GLuint count);
  template <is_vertex_layout T> void write(const T &vertex) {
    writeRaw(vertex.data(), sizeof(T), 1);
  }
  template <glm::has_value_ptr T> void write(const T &data) {
    writeRaw(glm::value_ptr(data), sizeof(T), 1);
  }
  template <typename T> void write(const T &list) {
    for (const auto &vertex : list)
      write(vertex);
  }

  void reset();
};
export struct VBOHeapHandleMapped : BufferObjectHeapHandle {
  GLuint count = 0, vertexSize = 0;

  VBOHeapHandleMapped(HeapBufferObject *parent, const GLuint offset,
                      const GLuint size, const GLuint vertexSize);

  void map(const void *data, const GLuint size, const GLuint count);
  template <typename T> T *map() const {
    return static_cast<T *>(
        glMapNamedBufferRange(parent->ID, offset, size, GL_MAP_WRITE_BIT));
  }
  void unmap(const GLuint count) {
    this->count = count;
    glUnmapNamedBuffer(parent->ID);
  }

  void writeRaw(const void *data, const GLuint size, const GLuint count);
  template <is_vertex_layout T> void write(const T &vertex) {
    writeRaw(vertex.data(), sizeof(T), 1);
  }
  template <glm::has_value_ptr T> void write(const T &data) {
    writeRaw(glm::value_ptr(data), sizeof(T), 1);
  }
  template <typename T> void write(const T &list) {
    using type = typename T::value_type;
    const GLuint size = static_cast<GLuint>(list.size());
    writeRaw(list.data(), static_cast<GLuint>(sizeof(type) * size), size);
  }
  template <typename T, GLuint L> void write(const T (&list)[L]) {
    writeRaw(list, static_cast<GLuint>(sizeof(T) * L), L);
  }

  void reset();
};
export struct EBOHeapHandle : BufferObjectHeapHandle {
  GLuint length = 0;

  EBOHeapHandle() = default;
  EBOHeapHandle(HeapBufferObject *parent, const GLuint offset,
                const GLuint size,
                const std::initializer_list<GLuint> &indices);
};

using VBOHeapHandle = VBOHeapHandleMapped;
export using VBOHandle = std::unique_ptr<VBOHeapHandle>;
export using EBOHandle = std::unique_ptr<EBOHeapHandle>;

namespace GL {
struct VertexBufferObject : ::HeapBufferObject {
  template <typename T> VBOHandle allocate(const GLuint count) {
    const GLuint size = count * static_cast<GLuint>(sizeof(T));
    if (size > MAX_SIZE)
      return {};
    for (auto current = freeList.begin(); current != freeList.cend();
         current++) {
      if (size > current->size)
        continue;

      const auto newSize = current->size - size;

      auto out = std::make_unique<VBOHeapHandle>(
          this, current->offset, size, static_cast<GLuint>(sizeof(T)));

      if (newSize == 0) {
        freeList.erase(current);
      } else {
        current->offset += size;
        current->size = newSize;
      }
      return out;
    }
    return {};
  }
};

struct ElementBufferObject : ::HeapBufferObject {
  EBOHandle allocate(const std::initializer_list<GLuint> &indices);
};
} // namespace GL

export template <typename T> struct BufferObjectAllocator {
  std::list<T> buffers;

  BufferObjectAllocator() { buffers.emplace_back(); }
};
export struct VBOAllocator : BufferObjectAllocator<GL::VertexBufferObject> {
  template <typename T = vertex_layout::pos> VBOHandle get(const GLuint count) {
    // println("VBO for {}", count);
    for (auto &buffer : buffers) {
      if (auto out = buffer.allocate<T>(count); out)
        return out;
    }
    return buffers.emplace_back().allocate<T>(count);
  }
};
export struct EBOAllocator : BufferObjectAllocator<GL::ElementBufferObject> {
  EBOHandle get(const std::initializer_list<GLuint> &indices);
};