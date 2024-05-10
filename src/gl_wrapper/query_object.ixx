module;

#include "util/gl.h"

export module query_object;

export struct query_object_t {
  GLuint ID;

  void init() { glGenQueries(1, &ID); }
};