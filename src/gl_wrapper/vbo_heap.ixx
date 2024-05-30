module;

#include "util/gl.h"

export module vao_heap;

import buffer_objects;
import vector;
import <memory>;

export struct VBOHeap {
  static constexpr GLsizeiptr BLOCK_SIZE = 64;

  struct VBOBlock;
  struct VBOHandle;

  struct VBOBlock {
    GLuint ID;

    std::vector<VBOHandle *> handles;

    VBOBlock() {
      glCreateBuffers(1, &ID);
      glNamedBufferStorage(ID, BLOCK_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
    }

    VBOHandle *get(const GLsizeiptr size) {
      if (handles.size() == 0) {
      } else {
      }
      return nullptr;
    }
  };

  struct VBOHandle {
    GLuint vboID;
    GLintptr offset;
    GLsizeiptr size;

    VBOHandle(const GLuint vboID, const GLintptr offset, const GLsizeiptr size)
        : vboID{vboID}, offset{offset}, size{size} {}
  };

  std::vector<VBOBlock> blocks{};

  VBOHeap() : blocks{1} {}
};