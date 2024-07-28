module;

#include "util/gl.h"

export module query_object;

export struct QueryObject {
  GLuint ID;

  void init() { glGenQueries(1, &ID); }
};