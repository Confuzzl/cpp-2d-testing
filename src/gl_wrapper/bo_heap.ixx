module;

#include "util/gl.h"

export module bo_heap;

import <vector>;
import <memory>;
import <list>;

import glm;
import vertex_layout;

struct BufferObject;
struct BufferObjectHeapHandle {
  BufferObject *parent = nullptr;
  GLuint offset = 0, size = 0;

  BufferObjectHeapHandle() = default;
  BufferObjectHeapHandle(BufferObject *parent, const GLuint offset,
                         const GLuint size);
  ~BufferObjectHeapHandle();
};
struct VBOHeapHandle : BufferObjectHeapHandle {
  GLuint count = 0, vertexSize = 0;

  VBOHeapHandle() = default;
  VBOHeapHandle(BufferObject *parent, const GLuint offset, const GLuint size,
                const GLuint vertexSize);

  void writeRaw(const void *data, const GLuint size);
  template <is_vertex_layout T> void write(const T &vertex) {
    writeRaw(vertex.data(), sizeof(T));
  }
  template <glm::has_value_ptr T> void write(const T &data) {
    writeRaw(glm::value_ptr(data), sizeof(T));
  }
  template <typename T> void writeList(const T &list) {
    for (const auto &vertex : list) {
      write(vertex);
    }
  }
};
struct EBOHeapHandle : BufferObjectHeapHandle {
  GLuint length = 0;

  EBOHeapHandle() = default;
  EBOHeapHandle(BufferObject *parent, const GLuint offset, const GLuint size,
                const GLuint length);

  void write(const std::initializer_list<GLuint> &indices);
};

using VBOHandle = std::unique_ptr<VBOHeapHandle>;
using EBOHandle = std::unique_ptr<EBOHeapHandle>;

struct BufferObject {
  static constexpr auto MAX_SIZE = 0xffffffu;

  GLuint ID;

  struct FreeBlock {
    GLuint offset, size;
  };
  using FreeList = std::list<FreeBlock>;
  FreeList freeList{{0, MAX_SIZE}};

  BufferObject();

  void free(const BufferObjectHeapHandle *handle);
  void coalesce(const FreeList::iterator &block);
  void coalesceRight(const FreeList::iterator &block);
};

struct VBO : BufferObject {
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

struct EBO : BufferObject {
  EBOHandle allocate(const std::initializer_list<GLuint> &indices);
};

template <typename T> struct BufferObjectAllocator {
  std::vector<T> buffers;

  BufferObjectAllocator() { buffers.reserve(8); }

  void init() { buffers.emplace_back(); }
};
struct VBOAllocator : BufferObjectAllocator<VBO> {
  template <typename T> VBOHandle get(const GLuint count) {
    for (auto &buffer : buffers) {
      if (auto out = buffer.allocate<T>(count); out)
        return out;
    }
    return buffers.emplace_back().allocate<T>(count);
  }

  template <GLuint count, typename T = vertex_layout::pos> VBOHandle get() {
    static VBOHandle cache = get<T>(count);
    return cache;
  }
};
struct EBOAllocator : BufferObjectAllocator<EBO> {
  EBOHandle get(const std::initializer_list<GLuint> &indices);
};

VBOAllocator VBO_HOLDER{};
EBOAllocator EBO_HOLDER{};
