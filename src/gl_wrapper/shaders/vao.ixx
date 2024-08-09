module;

#include "util/gl.h"

export module vao;

import vertex_layout;

export namespace GL {

template <is_vertex_layout T> struct VertexArrayObject {
  GLuint ID;

  VertexArrayObject() {
    glCreateVertexArrays(1, &ID);
    vertex_layout::enable<T>(ID);
  }
  ~VertexArrayObject() {
    if (ID)
      glDeleteVertexArrays(1, &ID);
  }
  VertexArrayObject(const VertexArrayObject &) = delete;
  VertexArrayObject(VertexArrayObject &&o) : ID{o.ID} { o.ID = 0; }
  VertexArrayObject &operator=(const VertexArrayObject &o) = delete;
  VertexArrayObject &operator=(VertexArrayObject &&o) {
    ID = o.ID;
    o.ID = 0;
    return *this;
  }
};
} // namespace GL
