module;

#include "util/gl.h"

module vao;

using namespace GL;

VertexArrayObject::VertexArrayObject() { glCreateVertexArrays(1, &ID); }
VertexArrayObject::~VertexArrayObject() {
  if (ID != -1)
    glDeleteVertexArrays(1, &ID);
}
VertexArrayObject::VertexArrayObject(VertexArrayObject &&o) : ID{o.ID} {
  o.ID = -1;
}
VertexArrayObject &VertexArrayObject::operator=(VertexArrayObject &&o) {
  ID = o.ID;
  o.ID = -1;
  return *this;
}