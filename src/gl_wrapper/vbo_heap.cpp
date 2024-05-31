module;

#include "util/gl.h"

module vbo_heap;

VBOHeap::VBOChunk::free_list::free_list() {
  free_block *block = new free_block{nullptr, nullptr, 0, BLOCK_SIZE};
  begin = block;
  end = block;
}
VBOHeap::VBOChunk::free_list::~free_list() {
  free_block *current = begin;
  while (current) {
    free_block *next = current->next;
    delete current;
    current = next;
  }
}

VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::prepend(const GLintptr offset,
                                      const GLsizeiptr size) {
  free_block *block = new free_block{end, nullptr, offset, size};
  begin->prev = block;
  begin = block;
  return block;
}
VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::append(const GLintptr offset,
                                     const GLsizeiptr size) {
  free_block *block = new free_block{end, nullptr, offset, size};
  end->next = block;
  end = block;
  return block;
}
VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::insertAfter(free_block *before,
                                          const GLintptr offset,
                                          const GLsizeiptr size) {
  free_block *block = new free_block{end, nullptr, offset, size};
  before->next->prev = block;
  before->next = block;
  return block;
}
void VBOHeap::VBOChunk::free_list::popFront() { remove(begin); }
void VBOHeap::VBOChunk::free_list::popBack() { remove(end); }
void VBOHeap::VBOChunk::free_list::remove(free_block *block) {
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

VBOHeap::VBOChunk::VBOChunk() : freeList{}, blockList{} {
  glCreateBuffers(1, &ID);
  glNamedBufferStorage(ID, BLOCK_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
}

const VBOHeap::VBOBlock *VBOHeap::VBOChunk::get(const GLsizeiptr size) {
  free_list::free_block *current = freeList.begin;
  while (current) {
    if (size > current->size) {
      current = current->next;
      continue;
    }
    GLsizeiptr newSize = current->size - size;
  }
  return nullptr;
}

const VBOHeap::VBOBlock &VBOHeap::get(const GLsizeiptr size) {
  for (VBOChunk &chunk : chunks) {
    const VBOBlock *block = chunk.get(size);
    if (block)
      return *block;
  }
  VBOChunk &newChunk = chunks.emplace_back();
  return *newChunk.get(size);
}