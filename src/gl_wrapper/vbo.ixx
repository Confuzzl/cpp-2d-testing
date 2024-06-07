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
  GLintptr offset = 0;
  VBO() { glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT); }
};
export struct VBOHandle {
  const GLint vboID;
  const GLsizeiptr dataSize;
  const GLintptr offset;
  GLintptr localOffset = 0;

  void write(const void *data) {
    glNamedBufferSubData(vboID, offset + localOffset, dataSize, data);
    localOffset += dataSize;
  }
  void reset() { localOffset = 0; }
};
export struct VBOHolder {
  std::vector<VBO> vbos{1};

  template <typename T> VBOHandle get(const GLsizeiptr size) {
    for (VBO &vbo : vbos) {
      if (vbo.offset + size > VBO::SIZE) {
        const VBO &v = vbos.emplace_back();
        return {.vboID = v.ID, .dataSize = sizeof(T), .offset = 0};
      }
      vbo.offset += size;
      return {.vboID = vbo.ID, .dataSize = sizeof(T), .offset = vbo.offset};
    }
  }
};