module;

#include "util/gl.h"

export module vao;

export namespace GL {
struct VertexArrayObject {
  GLuint ID;

  VertexArrayObject();
  ~VertexArrayObject();
  VertexArrayObject(const VertexArrayObject &) = delete;
  VertexArrayObject(VertexArrayObject &&o);
  VertexArrayObject &operator=(const VertexArrayObject &) = delete;
  VertexArrayObject &operator=(VertexArrayObject &&o);
};
} // namespace GL
