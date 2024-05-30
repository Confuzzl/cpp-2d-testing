module;

#include "util/gl.h"

module vbo_heap;

VBOHeap::VBOChunk::VBOChunk() : blockList{} {
  glCreateBuffers(1, &ID);
  glNamedBufferStorage(ID, BLOCK_SIZE, NULL, GL_DYNAMIC_STORAGE_BIT);
}
