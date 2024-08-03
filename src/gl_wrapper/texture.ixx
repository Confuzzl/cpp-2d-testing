module;

#include "util/gl.h"

export module texture;

import <string>;

import vector;

export namespace GL {
struct Texture {
  GLuint ID;

  Texture(const std::string &name);
  ~Texture();
  Texture(const Texture &) = delete;
  Texture(Texture &&o);
  Texture &operator=(const Texture &) = delete;
  Texture &operator=(Texture &&o);
};
} // namespace GL
