module;

#include "util/gl.h"

export module vbo_heap;

// import buffer_objects;
import vector;
import <memory>;
import <forward_list>;

export struct VBOHeap {
  static constexpr GLsizeiptr BLOCK_SIZE = 64;

  struct VBOBlock {
    GLuint vboID;
    GLintptr offset;
    GLsizeiptr size;
  };

  struct VBOChunk {
    GLuint ID;

    struct free_list {
      struct free_block {
        free_block *prev = nullptr, *next = nullptr;
        GLintptr offset;
        GLsizeiptr size;
      };

      free_block *begin = nullptr, *end = nullptr;

      free_list();
      ~free_list();

      free_block *prepend(const GLintptr offset, const GLsizeiptr size);
      free_block *append(const GLintptr offset, const GLsizeiptr size);
      free_block *insertAfter(free_block *before, const GLintptr offset,
                              const GLsizeiptr size);
      void popFront();
      void popBack();
      void remove(free_block *block);
    } freeList;

    std::vector<VBOBlock> blockList;

    VBOChunk();

    const VBOBlock *get(const GLsizeiptr size);
  };

  std::vector<VBOChunk> chunks;

  VBOHeap();

  const VBOBlock &get(const GLsizeiptr size);
};