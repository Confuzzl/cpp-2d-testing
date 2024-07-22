module;

#include "util/gl.h"

module buffer_object;

gl_buffer_obj::gl_buffer_obj() { glCreateBuffers(1, &ID); }

VBO::VBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }

VBOHandle::VBOHandle(const GLuint vboID, const GLintptr offset,
                     const std::size_t vertexSize)
    : vboID{vboID}, offset{offset}, vertexSize{vertexSize} {}

void VBOHandle::write(const void *data, const std::size_t size) {
  glNamedBufferSubData(vboID, offset, size, data);
  count++;
}

void VBOHandle::reset() { count = 0; }

std::vector<VBO> VBOHolder::vbos{};

void VBOHolder::init() { vbos.emplace_back(); }

EBO::EBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }

EBOHandle::EBOHandle(const GLuint eboID, const GLintptr offset,
                     const GLsizeiptr size,
                     const std::initializer_list<GLuint> &indices)
    : eboID{eboID}, count{static_cast<GLsizei>(indices.size())},
      offset{offset} {
  glNamedBufferSubData(eboID, offset, size, indices.begin());
}

std::vector<EBO> EBOHolder::ebos{};

void EBOHolder::init() { ebos.emplace_back(); }

EBOHandle EBOHolder::get(const std::initializer_list<GLuint> &indices) {
  const GLsizeiptr size = indices.size() * sizeof(GLuint);
  for (EBO &ebo : ebos) {
    if (ebo.offset + size > EBO::SIZE) {
      EBO &e = ebos.emplace_back();
      e.offset += size;
      return {e.ID, 0, size, indices};
    }
    const auto offset = ebo.offset;
    ebo.offset += size;
    return {ebo.ID, offset, size, indices};
  }
  return {};
}
