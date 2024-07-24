module;

#include "util/gl.h"

export module buffer_object;

import vector;
import glm;
import debug;
import vertex_layout;

struct gl_buffer_obj {
  GLuint ID;
  gl_buffer_obj();
};
// balls :3
export struct VBO : gl_buffer_obj {
  static constexpr GLsizeiptr SIZE = 0xffffff;
  GLintptr offset = 0;
  VBO();
};
export struct VBOHandle {
  GLuint vboID = 0;
  unsigned int count = 0;
  GLintptr offset = -1;
  std::size_t vertexSize = 0;

  VBOHandle() = default;
  VBOHandle(const GLuint vboID, const GLintptr offset,
            const std::size_t vertexSize);
  VBOHandle(const VBOHandle &) = delete;
  VBOHandle(VBOHandle &&) = default;
  VBOHandle &operator=(const VBOHandle &) = delete;
  VBOHandle &operator=(VBOHandle &&) = default;

  void write(const void *data, const std::size_t size);
  template <is_vertex_layout T> void write(const T &vertex) {
    write(vertex.data(), sizeof(T));
  }
  template <glm::has_value_ptr T> void write(const T &data) {
    write(glm::value_ptr(data), sizeof(T));
  }
  template <typename T> void writeList(const T &list) {
    for (const auto &vertex : list) {
      write(vertex);
    }
  }
  template <typename T, std::size_t L> void writeList(const T (&list)[L]) {
    for (const T &vertex : list) {
      write(vertex);
    }
  }

  void reset();
};

export struct VBOHolder {
  static std::vector<VBO> vbos;

  static void init();

  template <typename T> static VBOHandle get(const unsigned int count) {
    const GLsizeiptr capacity = sizeof(T) * count;
    for (VBO &vbo : vbos) {
      if (vbo.offset + capacity > VBO::SIZE) {
        VBO &v = vbos.emplace_back();
        v.offset += capacity;
        return {v.ID, 0, sizeof(T)};
      }
      const auto offset = vbo.offset;
      vbo.offset += capacity;
      return {vbo.ID, offset, sizeof(T)};
    }
    return {};
  }

  template <typename T, std::size_t L> static VBOHandle &cachedGet() {
    static VBOHandle out = get<T>(L);
    return out;
  }
};

export struct EBO : gl_buffer_obj {
  static constexpr GLsizeiptr SIZE = 0xffffff;
  GLintptr offset = 0;
  EBO();
};
export struct EBOHandle {
  GLuint eboID = 0;
  GLsizei count = -1;
  GLintptr offset = 0;

  EBOHandle() = default;
  EBOHandle(const GLuint eboID, const GLintptr offset, const GLsizeiptr size,
            const std::initializer_list<GLuint> &indices);
  EBOHandle(const EBOHandle &) = delete;
  EBOHandle(EBOHandle &&) = default;
};
export struct EBOHolder {
  static std::vector<EBO> ebos;

  static void init();

  static EBOHandle get(const std::initializer_list<GLuint> &indices);
};