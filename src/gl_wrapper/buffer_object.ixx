module;

#include "util/gl.h"

export module buffer_object;

import debug;

export namespace GL {
struct BufferObject {
  GLuint ID;

  BufferObject(const GLsizeiptr size, const GLbitfield flags,
               const void *data = nullptr) {
    glCreateBuffers(1, &ID);
    glNamedBufferStorage(ID, size, data, flags);
  }
  ~BufferObject() {
    if (ID)
      glDeleteBuffers(1, &ID);
  }
};

} // namespace GL