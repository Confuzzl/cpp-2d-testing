module;

#include "util/gl.h"

module bo_heap;

import debug;

BufferObjectHeapHandle::BufferObjectHeapHandle(HeapBufferObject *parent,
                                               const GLuint offset,
                                               const GLuint size)
    : parent{parent}, offset{offset}, size{size} {}
BufferObjectHeapHandle::~BufferObjectHeapHandle() {
  if (parent)
    parent->free(this);
}

VBOHeapHandle::VBOHeapHandle(HeapBufferObject *parent, const GLuint offset,
                             const GLuint size, const GLuint vertexSize)
    : BufferObjectHeapHandle(parent, offset, size), vertexSize{vertexSize} {}
void VBOHeapHandle::writeRaw(const void *data, const GLuint size) {
  glNamedBufferSubData(parent->ID, offset + count * vertexSize, size, data);
  if (++count * vertexSize > this->size)
    throw std::runtime_error{
        std::format("Overwrite at VBO handle at VBO {}", parent->ID)};
}
void VBOHeapHandle::reset() { count = 0; }

EBOHeapHandle::EBOHeapHandle(HeapBufferObject *parent, const GLuint offset,
                             const GLuint size,
                             const std::initializer_list<GLuint> &indices)
    : BufferObjectHeapHandle(parent, offset, size),
      length{static_cast<GLuint>(indices.size())} {
  glNamedBufferSubData(parent->ID, offset, size, indices.begin());
}

HeapBufferObject::HeapBufferObject() : GL::BufferObject(MAX_SIZE) {}
HeapBufferObject::HeapBufferObject(HeapBufferObject &&o)
    : GL::BufferObject(MAX_SIZE), freeList{std::move(o.freeList)} {
  o.ID = 0;
}
HeapBufferObject &HeapBufferObject::operator=(HeapBufferObject &&o) {
  ID = o.ID;
  freeList = std::move(o.freeList);
  o.ID = 0;
  return *this;
}

void HeapBufferObject::free(const BufferObjectHeapHandle *handle) {
  auto after = freeList.cbegin();
  while (after != freeList.cend() &&
         handle->offset + handle->size > after->offset) {
    after++;
  }
  coalesce(freeList.insert(after, {handle->offset, handle->size}));
}
void HeapBufferObject::coalesce(const FreeList::iterator &block) {
  coalesceRight(block);
  if (block != freeList.cbegin())
    coalesceRight(std::prev(block));
}
void HeapBufferObject::coalesceRight(const FreeList::iterator &block) {
  const auto next = std::next(block);
  if (block->offset + block->size == next->offset) {
    block->size += next->size;
    freeList.erase(next);
  }
}

EBOHandle GL::ElementBufferObject::allocate(
    const std::initializer_list<GLuint> &indices) {
  const GLuint size = static_cast<GLuint>(indices.size() * sizeof(GLuint));
  if (size > MAX_SIZE)
    return {};
  for (auto current = freeList.begin(); current != freeList.cend(); current++) {
    if (size > current->size)
      continue;

    const auto newSize = current->size - size;

    auto out =
        std::make_unique<EBOHeapHandle>(this, current->offset, size, indices);

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