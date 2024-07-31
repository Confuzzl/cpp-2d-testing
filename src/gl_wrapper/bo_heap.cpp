module;

#include "util/gl.h"

module bo_heap;

import debug;

BufferObjectHeapHandle::BufferObjectHeapHandle(BufferObject *parent,
                                               const GLuint offset,
                                               const GLuint size)
    : parent{parent}, offset{offset}, size{size} {}
BufferObjectHeapHandle::~BufferObjectHeapHandle() {
  if (parent)
    parent->free(this);
}

VBOHeapHandle::VBOHeapHandle(BufferObject *parent, const GLuint offset,
                             const GLuint size, const GLuint vertexSize)
    : BufferObjectHeapHandle(parent, offset, size), vertexSize{vertexSize} {}
void VBOHeapHandle::writeRaw(const void *data, const GLuint size) {
  glNamedBufferSubData(parent->ID, offset, size, data);
  if (++count * vertexSize > this->size)
    throw std::runtime_error{
        std::format("Overwrite at VBO handle at VBO {}", parent->ID)};
}
void VBOHeapHandle::reset() { count = 0; }

EBOHeapHandle::EBOHeapHandle(BufferObject *parent, const GLuint offset,
                             const GLuint size, const GLuint length)
    : BufferObjectHeapHandle(parent, offset, size), length{length} {}
void EBOHeapHandle::write(const std::initializer_list<GLuint> &indices) {
  glNamedBufferSubData(parent->ID, offset, size, indices.begin());
}

BufferObject::BufferObject() {
  glCreateBuffers(1, &ID);
  glNamedBufferStorage(ID, MAX_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
}

void BufferObject::free(const BufferObjectHeapHandle *handle) {
  auto after = freeList.cbegin();
  while (after != freeList.cend() &&
         handle->offset + handle->size > after->offset) {
    after++;
  }
  coalesce(freeList.insert(after, {handle->offset, handle->size}));
}
void BufferObject::coalesce(const FreeList::iterator &block) {
  coalesceRight(block);
  if (block != freeList.cbegin())
    coalesceRight(std::prev(block));
}
void BufferObject::coalesceRight(const FreeList::iterator &block) {
  auto next = std::next(block);
  if (block->offset + block->size == next->offset) {
    block->size += next->size;
    freeList.erase(next);
  }
}

EBOHandle EBO::allocate(const std::initializer_list<GLuint> &indices) {
  const GLuint size = static_cast<GLuint>(indices.size() * sizeof(GLuint));
  if (size > MAX_SIZE)
    return {};
  for (auto current = freeList.begin(); current != freeList.cend(); current++) {
    if (size > current->size)
      continue;

    const auto newSize = current->size - size;

    auto out = std::make_unique<EBOHeapHandle>(
        this, current->offset, size, static_cast<GLuint>(indices.size()));

    out->write(indices);
    if (newSize == 0) {
      freeList.erase(current);
    } else {
      current->offset += size;
      current->size = newSize;
    }
    return out;
  }
  return {};
}

EBOHandle EBOAllocator::get(const std::initializer_list<GLuint> &indices) {
  for (auto &buffer : buffers) {
    if (auto out = buffer.allocate(indices); out)
      return out;
  }
  return buffers.emplace_back().allocate(indices);
}