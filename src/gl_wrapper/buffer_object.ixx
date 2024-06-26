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

export struct VBO : gl_buffer_obj {
  static constexpr GLsizeiptr SIZE = 0xffffff;
  GLintptr offset = 0;
  VBO();
};
export struct VBOHandle {
  GLuint vboID = 0;
  unsigned int count = 0;
  GLintptr offset = -1;
  GLintptr localOffset = 0;
  std::size_t vertexSize;

  VBOHandle() = default;
  VBOHandle(const GLuint vboID, const GLintptr offset,
            const std::size_t vertexSize);
  VBOHandle(const VBOHandle &) = delete;
  VBOHandle(VBOHandle &&) = default;

  template <is_vertex_layout T> void write(const T &vertex) {
    glNamedBufferSubData(vboID, offset + localOffset, sizeof(T), vertex.data());
    localOffset += sizeof(T);
    count++;
  }
  template <glm::has_value_ptr T> void write(const T &data) {
    glNamedBufferSubData(vboID, offset + localOffset, sizeof(data),
                         glm::value_ptr(data));
    localOffset += sizeof(data);
    count++;
  }

  void reset();
};

export struct VBOHolder {
  static std::vector<VBO> vbos;
  static std::vector<VBOHandle> handles;

  static VBOHandle *POINT, *LINE, *TRI, *QUAD;

  static void init();

  template <typename T>
  static std::pair<VBOHandle *, int> getPair(const unsigned int count) {
    const GLsizeiptr capacity = sizeof(T) * count;
    for (VBO &vbo : vbos) {
      if (vbo.offset + capacity > VBO::SIZE) {
        VBO &v = vbos.emplace_back();
        v.offset += capacity;
        return {&handles.emplace_back(v.ID, 0, sizeof(T)),
                static_cast<int>(handles.size() - 1)};
      }
      const auto offset = vbo.offset;
      vbo.offset += capacity;
      return {&handles.emplace_back(vbo.ID, offset, sizeof(T)),
              static_cast<int>(handles.size() - 1)};
    }
    return {nullptr, -1};
  }
  template <typename T> static VBOHandle &getHandle(const unsigned int count) {
    return *getPair<T>(count).first;
  }
  template <typename T> static int getIndex(const unsigned int count) {
    return getPair<T>(count).second;
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
  static std::vector<EBOHandle> handles;

  static void init();

  static std::pair<EBOHandle *, int>
  get(const std::initializer_list<GLuint> &indices);
  static EBOHandle &getHandle(const std::initializer_list<GLuint> &indices);
  static int getIndex(const std::initializer_list<GLuint> &indices);
};