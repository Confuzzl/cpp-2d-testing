module;

#include "util/gl.h"

export module vbo_heap;

// import buffer_objects;
import vector;
import <memory>;
import <forward_list>;
import <optional>;

export struct VBOHeap {
  static constexpr GLsizeiptr BLOCK_SIZE = 64;

  struct VBOChunk;

  struct VBOHandle {
    VBOChunk *chunk;
    GLintptr offset;
    GLsizeiptr size;

    ~VBOHandle();
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

      void setBegin(free_block *block);
      void setEnd(free_block *block);

      free_block *prepend(const GLintptr offset, const GLsizeiptr size);
      free_block *append(const GLintptr offset, const GLsizeiptr size);
      free_block *insertAfter(free_block *before, const GLintptr offset,
                              const GLsizeiptr size);
      free_block *insertBefore(free_block *after, const GLintptr offset,
                               const GLsizeiptr size);

      free_block *coalesce(free_block *block);

      void popFront();
      void popBack();
      void remove(free_block *block);
    } freeList;

    VBOChunk();

    std::optional<VBOHandle> get(const GLsizeiptr size);
    void free(VBOHandle *handle);
  };

  std::vector<VBOChunk> chunks;

  VBOHeap();

  VBOHandle get(const GLsizeiptr size);
};