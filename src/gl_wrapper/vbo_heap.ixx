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

      free_block *begin = nullptr;
      free_block *end = nullptr;

      free_list() {
        free_block *block = new free_block{end, nullptr, 0, BLOCK_SIZE};
        begin = block;
        end = block;
      }
      ~free_list() {
        free_block *current = begin;
        while (current) {
          free_block *next = current->next;
          delete current;
          current = next;
        }
      }

      free_block *prepend(const GLintptr offset, const GLsizeiptr size) {
        free_block *block = new free_block{end, nullptr, offset, size};
        begin->prev = block;
        begin = block;
        return block;
      }
      free_block *append(const GLintptr offset, const GLsizeiptr size) {
        free_block *block = new free_block{end, nullptr, offset, size};
        end->next = block;
        end = block;
        return block;
      }
      free_block *insertAfter(free_block *before, const GLintptr offset,
                              const GLsizeiptr size) {
        free_block *block = new free_block{end, nullptr, offset, size};
        before->next->prev = block;
        before->next = block;
        return block;
      }
      void remove(free_block *block) {
        free_block *prev = block->prev, *next = block->next;
        if (block == begin) {
          next->prev = nullptr;
          begin = next;
        } else if (block == end) {
          prev->next = nullptr;
          end = prev;
        } else {
          prev->next = next;
          next->prev = prev;
        }
        delete block;
      }
    };

    std::vector<VBOBlock> blockList;

    VBOChunk();

    const VBOBlock &get() const;
  };

  std::vector<VBOChunk> blocks{};

  VBOHeap() : blocks{1} {}
};