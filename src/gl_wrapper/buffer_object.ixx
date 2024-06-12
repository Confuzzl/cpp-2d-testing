module;

#include "util/gl.h"

export module buffer_object;

import vector;
import glm;
import debug;

struct gl_buffer_obj {
  GLuint ID;
  gl_buffer_obj();
};

export struct VBO : gl_buffer_obj {
  static constexpr GLsizeiptr SIZE = 0xffffff;
  GLintptr offset = 0;
  VBO();
};
export struct VBOHandle {
  GLuint vboID;
  unsigned int count = 0;
  GLintptr offset;
  std::size_t vertexSize;
  GLintptr localOffset = 0;

  VBOHandle(const GLuint vboID, const GLintptr offset,
            const std::size_t vertexSize);

  template <typename T> void writePartial(const T &data) {
    glNamedBufferSubData(vboID, offset + localOffset, sizeof(data),
                         glm::value_ptr(data));
    localOffset += sizeof(data);
  }
  template <typename T> void write(const T &data) {
    writePartial(data);
    count++;
  }

  void reset();
};

export struct VBOHolder {
  std::vector<VBO> vbos{1};

  void init();

  VBOHandle get(const std::size_t vertexSize, const unsigned int count);
  template <typename T> VBOHandle get(const unsigned int count) {
    return get(sizeof(T), count);
  }
};

export struct EBO : gl_buffer_obj {
  static constexpr GLsizeiptr SIZE = 0xffffff;
  GLintptr offset = 0;
  EBO();
};
export struct EBOHandle {
  GLuint eboID;
  GLsizei count;
  GLintptr offset;

  EBOHandle(const GLuint eboID, const GLintptr offset, const GLsizeiptr size,
            const std::initializer_list<GLuint> &indices);
};
export struct EBOHolder {
  std::vector<EBO> ebos{1};

  EBOHandle get(const std::initializer_list<GLuint> &indices);
};