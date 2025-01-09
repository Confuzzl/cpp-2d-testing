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

VBOHeapHandleSubData::VBOHeapHandleSubData(HeapBufferObject *parent,
                                           const GLuint offset,
                                           const GLuint size,
                                           const GLuint vertexSize)
    : BufferObjectHeapHandle(parent, offset, size), vertexSize{vertexSize} {
  // auto ptr = glMapNamedBufferRange(parent->ID, offset, size,
  // GL_MAP_WRITE_BIT); glUnmapNamedBuffer(parent->ID);
}
void VBOHeapHandleSubData::writeRaw(const void *data, const GLuint size,
                                    const GLuint count) {
  glNamedBufferSubData(parent->ID, offset + this->count * vertexSize, size,
                       data);
  this->count += count;
  if (this->count * vertexSize > this->size)
    throw std::runtime_error{
        std::format("Overwrite at VBO handle at VBO {}", parent->ID)};
}
void VBOHeapHandleSubData::reset() { count = 0; }

VBOHeapHandleMapped::VBOHeapHandleMapped(HeapBufferObject *parent,
                                         const GLuint offset, const GLuint size,
                                         const GLuint vertexSize)
    : BufferObjectHeapHandle(parent, offset, size), vertexSize{vertexSize} {}
void VBOHeapHandleMapped::map(const void *data, const GLuint size,
                              const GLuint count) {
  auto ptr = glMapNamedBufferRange(parent->ID, offset, size, GL_MAP_WRITE_BIT);
  std::memcpy(ptr, data, size);
  glUnmapNamedBuffer(parent->ID);
  this->count += count;
}
void VBOHeapHandleMapped::writeRaw(const void *data, const GLuint size,
                                   const GLuint count) {
  glNamedBufferSubData(parent->ID, offset + this->count * vertexSize, size,
                       data);
  this->count += count;
  if (this->count * vertexSize > this->size)
    throw std::runtime_error{
        std::format("Overwrite at VBO handle at VBO {}", parent->ID)};
}
void VBOHeapHandleMapped::reset() {
  count = 0;
  // glUnmapNamedBuffer(parent->ID);
}

EBOHeapHandle::EBOHeapHandle(HeapBufferObject *parent, const GLuint offset,
                             const GLuint size,
                             const std::initializer_list<GLuint> &indices)
    : BufferObjectHeapHandle(parent, offset, size),
      length{static_cast<GLuint>(indices.size())} {
  glNamedBufferSubData(parent->ID, offset, size, indices.begin());
}

HeapBufferObject::HeapBufferObject()
    : GL::BufferObject(MAX_SIZE, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT) {}
HeapBufferObject::HeapBufferObject(HeapBufferObject &&o)
    : GL::BufferObject(MAX_SIZE, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT),
      freeList{std::move(o.freeList)} {
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
void HeapBufferObject::coalesce(const free_list::iterator &block) {
  coalesceRight(block);
  if (block != freeList.cbegin())
    coalesceRight(std::prev(block));
}
void HeapBufferObject::coalesceRight(const free_list::iterator &block) {
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