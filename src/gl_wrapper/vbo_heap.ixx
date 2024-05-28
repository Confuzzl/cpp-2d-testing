module;

#include "util/gl.h"

export module vao_heap;

import buffer_objects;

export struct VBOHeap {
  struct VBOHandle {
    GLintptr offset;
    GLsizeiptr size;
  };
};