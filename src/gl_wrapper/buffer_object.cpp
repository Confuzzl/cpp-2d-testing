module;

#include "util/gl.h"

module buffer_object;

gl_buffer_obj::gl_buffer_obj() { glCreateBuffers(1, &ID); }

VBO::VBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }

VBOHandle::VBOHandle(const GLuint vboID, const GLintptr offset,
                     const std::size_t vertexSize)
    : vboID{vboID}, offset{offset}, vertexSize{vertexSize} {}

void VBOHandle::reset() {
  count = 0;
  localOffset = 0;
}

void VBOHolder::init() { vbos.emplace_back(); }
VBOHandle VBOHolder::get(const std::size_t vertexSize,
                         const unsigned int count) {
  const GLsizeiptr capacity = vertexSize * count;
  for (VBO &vbo : vbos) {
    if (vbo.offset + capacity > VBO::SIZE) {
      VBO &v = vbos.emplace_back();
      VBOHandle out = {v.ID, 0, vertexSize};
      v.offset += capacity;
      return out;
    }
    VBOHandle out = {vbo.ID, vbo.offset, vertexSize};
    println("vbohandle: {} {} {}*{}={}", vbo.ID, vbo.offset, vertexSize, count,
            capacity);
    vbo.offset += capacity;
    return out;
  }
  return {};
}

EBO::EBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }

EBOHandle::EBOHandle(const GLuint eboID, const GLintptr offset,
                     const GLsizeiptr size,
                     const std::initializer_list<GLuint> &indices)
    : eboID{eboID}, count{static_cast<GLsizei>(indices.size())},
      offset{offset} {
  glNamedBufferSubData(eboID, offset, size, indices.begin());
}

void EBOHolder::init() { ebos.emplace_back(); }
EBOHandle EBOHolder::get(const std::initializer_list<GLuint> &indices) {
  const GLsizeiptr size = indices.size() * sizeof(GLuint);
  for (EBO &ebo : ebos) {
    if (ebo.offset + size > EBO::SIZE) {
      EBO &e = ebos.emplace_back();
      EBOHandle out = {e.ID, 0, size, indices};
      e.offset += size;
      return out;
    }
    EBOHandle out = {ebo.ID, ebo.offset, size, indices};
    ebo.offset += size;
    return out;
  }
  return {};
}