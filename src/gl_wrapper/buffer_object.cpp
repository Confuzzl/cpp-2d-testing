module;

#include "util/gl.h"

module buffer_object;

gl_buffer_obj::gl_buffer_obj() { glCreateBuffers(1, &ID); }

VBO::VBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }

VBOHandle::VBOHandle(const GLuint vboID, const GLintptr offset,
                     const std::size_t vertexSize)
    : vboID{vboID}, offset{offset}, vertexSize{vertexSize} {}

void VBOHandle::write(const void *data, const std::size_t size) {
  glNamedBufferSubData(vboID, offset + localOffset, size, data);
  localOffset += size;
  count++;
}

void VBOHandle::reset() {
  count = 0;
  localOffset = 0;
}

std::vector<VBO> VBOHolder::vbos{};
std::vector<VBOHandle> VBOHolder::handles{};

void VBOHolder::init() {
  vbos.emplace_back();
  VBOHolder::getIndex<vertex_layout::pos>(1);
  VBOHolder::getIndex<vertex_layout::pos>(2);
  VBOHolder::getIndex<vertex_layout::pos>(3);
  VBOHolder::getIndex<vertex_layout::pos>(4);
}

EBO::EBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }

EBOHandle::EBOHandle(const GLuint eboID, const GLintptr offset,
                     const GLsizeiptr size,
                     const std::initializer_list<GLuint> &indices)
    : eboID{eboID}, count{static_cast<GLsizei>(indices.size())},
      offset{offset} {
  glNamedBufferSubData(eboID, offset, size, indices.begin());
}

std::vector<EBO> EBOHolder::ebos{};
std::vector<EBOHandle> EBOHolder::handles{};

void EBOHolder::init() { ebos.emplace_back(); }

std::pair<EBOHandle *, int>
EBOHolder::get(const std::initializer_list<GLuint> &indices) {
  const GLsizeiptr size = indices.size() * sizeof(GLuint);
  for (EBO &ebo : ebos) {
    if (ebo.offset + size > EBO::SIZE) {
      EBO &e = ebos.emplace_back();
      e.offset += size;
      return {&handles.emplace_back(e.ID, 0, size, indices),
              static_cast<int>(handles.size() - 1)};
    }
    const auto offset = ebo.offset;
    ebo.offset += size;
    return {&handles.emplace_back(ebo.ID, offset, size, indices),
            static_cast<int>(handles.size() - 1)};
  }
  return {};
}
EBOHandle &EBOHolder::getHandle(const std::initializer_list<GLuint> &indices) {
  return *get(indices).first;
}
int EBOHolder::getIndex(const std::initializer_list<GLuint> &indices) {
  return get(indices).second;
}
