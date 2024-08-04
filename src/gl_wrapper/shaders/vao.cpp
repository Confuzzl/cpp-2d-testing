module;

#include "util/gl.h"

module vao;

using namespace GL;

VertexArrayObject::VertexArrayObject() { glCreateVertexArrays(1, &ID); }
VertexArrayObject::~VertexArrayObject() {
  if (ID)
    glDeleteVertexArrays(1, &ID);
}
VertexArrayObject::VertexArrayObject(VertexArrayObject &&o) : ID{o.ID} {
  o.ID = 0;
}
VertexArrayObject &VertexArrayObject::operator=(VertexArrayObject &&o) {
  ID = o.ID;
  o.ID = 0;
  return *this;
}