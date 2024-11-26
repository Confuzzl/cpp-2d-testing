module;

#include "util/gl.h"

export module buffer_object;

import debug;

export namespace GL {
struct BufferObject {
  GLuint ID;

  BufferObject(const GLsizeiptr size, const void *data = nullptr) {
    glCreateBuffers(1, &ID);
    glNamedBufferStorage(ID, size, data, GL_DYNAMIC_STORAGE_BIT);
  }
  ~BufferObject() {
    if (ID)
      glDeleteBuffers(1, &ID);
  }
};

} // namespace GL