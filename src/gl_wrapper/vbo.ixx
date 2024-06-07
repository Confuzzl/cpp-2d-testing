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
  GLuint vboID;
  GLsizeiptr dataSize;
  GLintptr offset;
  GLintptr localOffset = 0;

  void write(const void *data) {
    glNamedBufferSubData(vboID, offset + localOffset, dataSize, data);
    localOffset += dataSize;
  }
  void reset() { localOffset = 0; }
};
export struct VBOHolder {
  std::vector<VBO> vbos{1};

  VBOHandle get(const GLsizeiptr dataSize, const unsigned int count) {
    const GLsizeiptr size = count * dataSize;
    for (VBO &vbo : vbos) {
      if (vbo.offset + size > VBO::SIZE) {
        const VBO &v = vbos.emplace_back();
        return {.vboID = v.ID, .dataSize = dataSize, .offset = 0};
      }
      VBOHandle out = {
          .vboID = vbo.ID, .dataSize = dataSize, .offset = vbo.offset};
      vbo.offset += size;
      return out;
    }
    // unreachable
    return {};
  }
} VBO_HOLDER;

export struct EBO : gl_buffer_obj {
  EBO(const std::vector<GLuint> &indices) {
    glNamedBufferData(ID, indices.size() * sizeof(GLuint), indices.data(),
                      GL_STATIC_DRAW);
  }
};