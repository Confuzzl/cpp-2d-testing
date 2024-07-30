module;

#include "util/gl.h"

export module query_object;

export struct QueryObject {
  GLuint ID;

  QueryObject() {
    glfwInit();
    glGenQueries(1, &ID);
  }

  void init() {}
};