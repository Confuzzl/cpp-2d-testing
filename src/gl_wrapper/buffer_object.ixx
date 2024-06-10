module;

#include "util/gl.h"

export module vbo;

import vector;
import glm;

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
  GLintptr offset;
  std::size_t vertexSize;

  unsigned int count;
  GLintptr localOffset = 0;

  template <typename T> void writePartial(const T &data) {
    glNamedBufferSubData(vboID, offset + localOffset, sizeof(data),
                         glm::value_ptr(data));
    localOffset += sizeof(data);
  }
  template <typename T> void write(const T &data) {
    writePartial(data);
    count++;
  }

  // void write(const void *data) {
  //   glNamedBufferSubData(vboID, offset + localOffset, dataSize, data);
  //   localOffset += dataSize;
  // }
  void reset() { localOffset = 0; }
};
export struct VBOHolder {
  std::vector<VBO> vbos{1};

  VBOHandle get(const std::size_t vertexSize, const unsigned int count) {
    const GLsizeiptr capacity = vertexSize * count;
    for (VBO &vbo : vbos) {
      if (vbo.offset + capacity > VBO::SIZE) {
        VBO &v = vbos.emplace_back();
        VBOHandle out = {.vboID = v.ID, .offset = 0, .vertexSize = vertexSize};
        v.offset += capacity;
        return out;
      }
      VBOHandle out = {
          .vboID = vbo.ID, .offset = vbo.offset, .vertexSize = vertexSize};
      vbo.offset += capacity;
      return out;
    }
    // unreachable
    return {};
  }
} VBO_HOLDER;

export struct EBO : gl_buffer_obj {
  static constexpr GLsizeiptr SIZE = 0xffffff;
  GLintptr offset = 0;
  EBO() {
    glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
    // glNamedBufferData(ID, indices.size() * sizeof(GLuint), indices.data(),
    //                   GL_STATIC_DRAW);
  }
};
export struct EBOHandle {
  GLuint eboID;
  GLintptr offset;
  unsigned int count;

  EBOHandle(const GLuint eboID, const GLintptr offset, const unsigned int count,
            const std::initializer_list<GLuint> &indices)
      : eboID{eboID}, offset{offset}, count{count} {
    glNamedBufferData(eboID, count * sizeof(GLuint), indices.begin(),
                      GL_STATIC_DRAW);
  }
};
export struct EBOHolder {
  std::vector<EBO> ebos{1};

  EBOHandle get(const std::initializer_list<GLuint> &indices) {
    const unsigned int count = static_cast<unsigned int>(indices.size());
    const GLsizeiptr size = count * sizeof(GLuint);
    for (EBO &ebo : ebos) {
      if (ebo.offset + size > EBO::SIZE) {
        EBO &e = ebos.emplace_back();
        EBOHandle out = {e.ID, 0, count, indices};
        e.offset += size;
        return out;
      }
      EBOHandle out = {ebo.ID, ebo.offset, count, indices};
      ebo.offset += size;
      return out;
    }
    return {0, -1, 0, {}};
  }
} EBO_HOLDER;