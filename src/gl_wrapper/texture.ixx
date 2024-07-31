module;

#include "util/gl.h"

export module texture;

import <string>;

import vector;

export namespace tex {
struct texture {
  GLuint ID = 0;

  texture(const std::string &name);
  ~texture();
};
} // namespace tex