module;

#include "util/gl.h"

module vbo_heap;

VBOHeap::VBOHandle::~VBOHandle() { chunk->free(this); }

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

void VBOHeap::VBOChunk::free_list::setBegin(free_block *block) {
  block->prev = nullptr;
  block->next = begin;
  begin->prev = block;
  begin = block;
}
void VBOHeap::VBOChunk::free_list::setEnd(free_block *block) {
  block->next = nullptr;
  block->prev = end;
  end->next = block;
  end = block;
}

VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::prepend(const GLintptr offset,
                                      const GLsizeiptr size) {
  free_block *block = new free_block{nullptr, nullptr, offset, size};
  setBegin(block);
  return block;
}
VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::append(const GLintptr offset,
                                     const GLsizeiptr size) {
  free_block *block = new free_block{end, nullptr, offset, size};
  setEnd(block);
  return block;
}
VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::insertAfter(free_block *before,
                                          const GLintptr offset,
                                          const GLsizeiptr size) {
  free_block *block = new free_block{nullptr, nullptr, offset, size};
  if (before == end) {
    setEnd(block);
  } else {
    before->next = block;
    block->prev = before;
    free_block *after = before->next;
    block->next = after;
    after->prev = block;
  }
  return block;
}
VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::insertBefore(free_block *after,
                                           const GLintptr offset,
                                           const GLsizeiptr size) {
  free_block *block = new free_block{nullptr, nullptr, offset, size};
  if (after == begin) {
    setBegin(block);
  } else {
    after->prev = block;
    block->next = after;
    free_block *before = after->prev;
    block->prev = before;
    before->next = block;
  }
  return block;
}

VBOHeap::VBOChunk::free_list::free_block *
VBOHeap::VBOChunk::free_list::coalesce(free_block *block) {
  free_block *before = block->prev, *after = block->next;
  if (before && before->offset + before->size == block->offset) {
    block->offset = before->offset;
    block->size += before->size;
    before->prev->next = block;
    delete before;
  }
  if (after && block->offset + block->size == after->offset) {
    block->size += after->size;
    after->next->prev = block;
    delete after;
  }
  return block;
}

void VBOHeap::VBOChunk::free_list::popFront() { remove(begin); }
void VBOHeap::VBOChunk::free_list::popBack() { remove(end); }
void VBOHeap::VBOChunk::free_list::remove(free_block *block) {
  free_block *prev = block->prev, *next = block->next;
  if (block == begin) {
    setBegin(next);
  } else if (block == end) {
    setEnd(prev);
  } else {
    prev->next = next;
    next->prev = prev;
  }
  delete block;
}

VBOHeap::VBOChunk::VBOChunk() : freeList{} {
  glCreateBuffers(1, &ID);
  glNamedBufferStorage(ID, BLOCK_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
}

std::optional<VBOHeap::VBOHandle>
VBOHeap::VBOChunk::get(const GLsizeiptr size) {
  free_list::free_block *current = freeList.begin;
  while (current) {
    GLsizeiptr newSize = current->size - size;
    if (newSize < 0) {
      current = current->next;
      continue;
    }

    VBOHandle handle{this, current->offset, size};
    if (newSize == 0) {
      freeList.remove(current);
    } else {
      current->offset += size;
      current->size = newSize;
    }
    return handle;
  }
  return std::nullopt; // no frees
}
void VBOHeap::VBOChunk::free(VBOHandle *handle) {
  const auto [chunk, offset, size] = *handle;
  free_list::free_block *before = freeList.begin;
  // insert new free after free with the largest offset smaller than handle
  // offset
  while (before->next && before->next->offset < offset) {
    before = before->next;
  }
  free_list::free_block *block = freeList.insertBefore(before, offset, size);
  freeList.coalesce(block);
}

VBOHeap::VBOHeap() : chunks{} {}

VBOHeap::VBOHandle VBOHeap::get(const GLsizeiptr size) {
  for (VBOChunk &chunk : chunks) {
    std::optional<VBOHandle> handle = chunk.get(size);
    if (handle)
      return *handle;
  }
  VBOChunk &newChunk = chunks.emplace_back();
  return *newChunk.get(size);
}