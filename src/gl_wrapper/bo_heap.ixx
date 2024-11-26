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

export struct VBOHeapHandle : BufferObjectHeapHandle {
  GLuint count = 0, vertexSize = 0;

  VBOHeapHandle() = default;
  VBOHeapHandle(HeapBufferObject *parent, const GLuint offset,
                const GLuint size, const GLuint vertexSize);

  void writeRaw(const void *data, const GLuint size);
  template <is_vertex_layout T> void write(const T &vertex) {
    writeRaw(vertex.data(), sizeof(T));
  }
  template <glm::has_value_ptr T> void write(const T &data) {
    writeRaw(glm::value_ptr(data), sizeof(T));
  }
  template <typename T> void write(const T &list) {
    for (const auto &vertex : list)
      write(vertex);
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

// owning VBO handle
export using VBOHandle = std::unique_ptr<VBOHeapHandle>;
// owning EBO handle
export using EBOHandle = std::unique_ptr<EBOHeapHandle>;

struct HeapBufferObject : GL::BufferObject {
  static constexpr auto MAX_SIZE = 0xffffffu;

  struct FreeBlock {
    GLuint offset, size;
  };
  using FreeList = std::list<FreeBlock>;
  FreeList freeList{{0, MAX_SIZE}};

  HeapBufferObject();
  ~HeapBufferObject() = default;
  HeapBufferObject(const HeapBufferObject &) = delete;
  HeapBufferObject(HeapBufferObject &&o);
  HeapBufferObject &operator=(const HeapBufferObject &) = delete;
  HeapBufferObject &operator=(HeapBufferObject &&o);

  void free(const BufferObjectHeapHandle *handle);
  void coalesce(const FreeList::iterator &block);
  void coalesceRight(const FreeList::iterator &block);
};

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