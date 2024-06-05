module;

#include "util/gl.h"

export module vbo;

import vector;

struct gl_buffer_obj {
  GLuint ID;
  gl_buffer_obj() { glCreateBuffers(1, &ID); }
};

export struct VBO : gl_buffer_obj {
  static constexpr GLsizeiptr SIZE = 0xffffff;
  VBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }
};
export struct VBOHandle {
  GLint vboID;
  GLsizeiptr dataSize;
  GLintptr offset = 0;

  void write(const void *data) {
    glNamedBufferSubData(vboID, offset, dataSize, data);
    offset += dataSize;
  }
  void reset() { offset = 0; }
};
export struct VBOHolder {
  std::vector<VBO> vbos;

  VBOHandle get(const GLsizeiptr size) {
    for (VBO &vbo : vbos) {
    }
  }
};