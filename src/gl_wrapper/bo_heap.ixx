module;

#include "util/gl.h"

export module bo_heap;

import <vector>;
import <memory>;
import <list>;

import glm;
import vertex_layout;

export namespace heap {
struct buffer_object;
struct raw_handle {
  buffer_object *parent = nullptr;
  GLuint offset = 0, size = 0;

  raw_handle() = default;
  raw_handle(buffer_object *parent, const GLuint offset, const GLuint size);
  ~raw_handle();
};
struct raw_vbo_handle : raw_handle {
  GLuint count = 0, vertexSize = 0;

  raw_vbo_handle() = default;
  raw_vbo_handle(buffer_object *parent, const GLuint offset, const GLuint size,
                 const GLuint count, const GLuint vertexSize);

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
struct raw_ebo_handle : raw_handle {
  using raw_handle::raw_handle;

  void write(const std::initializer_list<GLuint> &indices);
};

using vbo_handle = std::unique_ptr<raw_vbo_handle>;
using ebo_handle = std::unique_ptr<raw_ebo_handle>;

struct buffer_object {
  static constexpr auto SIZE = 0xffffffu;

  GLuint ID;

  struct free_block {
    GLuint offset, size;
  };
  using free_list = std::list<free_block>;
  free_list freeList{{0, SIZE}};

  buffer_object();

  void free(const raw_handle *handle);
  void coalesce(const free_list::iterator &block);
  void coalesceRight(const free_list::iterator &block);
};

struct vbo : buffer_object {
  template <typename T> vbo_handle allocate(const GLuint count) {
    const GLuint size = count * static_cast<GLuint>(sizeof(T));
    if (size > SIZE)
      return {};
    for (auto current = freeList.begin(); current != freeList.cend();
         current++) {
      if (size > current->size)
        continue;

      const auto newSize = current->size - size;

      auto out = std::make_unique<raw_vbo_handle>(
          this, current->offset, size, count, static_cast<GLuint>(sizeof(T)));

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

struct ebo : buffer_object {
  ebo_handle allocate(const std::initializer_list<GLuint> &indices);
};

template <typename T> struct allocator {
  std::vector<T> buffers;

  allocator() { buffers.reserve(8); }

  void init() { buffers.emplace_back(); }
};
struct vbo_allocator : allocator<vbo> {
  template <typename T> vbo_handle get(const GLuint count) {
    for (auto &buffer : buffers) {
      if (auto out = buffer.allocate<T>(count); out)
        return out;
    }
    return buffers.emplace_back().allocate<T>(count);
  }

  template <GLuint count, typename T = vertex_layout::pos> vbo_handle get() {
    static vbo_handle cache = get<T>(count);
    return cache;
  }
};
struct ebo_allocator : allocator<ebo> {
  ebo_handle get(const std::initializer_list<GLuint> &indices);
};

vbo_allocator VBO_HOLDER{};
ebo_allocator EBO_HOLDER{};
} // namespace heap