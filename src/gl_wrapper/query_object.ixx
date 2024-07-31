module;

#include "util/gl.h"

export module query_object;

export struct QueryObject {
  GLuint ID;

  QueryObject() { glGenQueries(1, &ID); }
};