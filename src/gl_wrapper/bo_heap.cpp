module;

#include "util/gl.h"

module bo_heap;

using namespace heap;

raw_handle::raw_handle(buffer_object *parent, const GLuint offset,
                       const GLuint size)
    : parent{parent}, offset{offset}, size{size} {}
raw_handle::~raw_handle() {
  if (parent) {
    parent->free(this);
  }
}
raw_vbo_handle::raw_vbo_handle(buffer_object *parent, const GLuint offset,
                               const GLuint size, const GLuint count,
                               const GLuint vertexSize)
    : raw_handle(parent, offset, size), count{count}, vertexSize{vertexSize} {}
void raw_vbo_handle::writeRaw(const void *data, const GLuint size) {
  glNamedBufferSubData(parent->ID, offset, size, data);
}
void raw_ebo_handle::write(const std::initializer_list<GLuint> &indices) {
  glNamedBufferSubData(parent->ID, offset, size, indices.begin());
}

buffer_object::buffer_object() {
  glCreateBuffers(1, &ID);
  glNamedBufferStorage(ID, SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
}

void buffer_object::free(const raw_handle *handle) {
  auto after = freeList.cbegin();
  while (after != freeList.cend() &&
         handle->offset + handle->size > after->offset) {
    after++;
  }
  coalesce(freeList.insert(after, {handle->offset, handle->size}));
}
void buffer_object::coalesce(const free_list::iterator &block) {
  coalesceRight(block);
  if (block != freeList.cbegin())
    coalesceRight(std::prev(block));
}
void buffer_object::coalesceRight(const free_list::iterator &block) {
  auto next = std::next(block);
  if (block->offset + block->size == next->offset) {
    block->size += next->size;
    freeList.erase(next);
  }
}

ebo_handle ebo::allocate(const std::initializer_list<GLuint> &indices) {
  const GLuint size = static_cast<GLuint>(indices.size() * sizeof(GLuint));
  if (size > SIZE)
    return {};
  for (auto current = freeList.begin(); current != freeList.cend(); current++) {
    if (size > current->size)
      continue;

    const auto newSize = current->size - size;

    auto out = std::make_unique<raw_ebo_handle>(this, current->offset, size);

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

ebo_handle ebo_allocator::get(const std::initializer_list<GLuint> &indices) {
  for (auto &buffer : buffers) {
    if (auto out = buffer.allocate(indices); out)
      return out;
  }
  return buffers.emplace_back().allocate(indices);
}