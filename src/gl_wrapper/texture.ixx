module;

#include "util/gl.h"

export module texture;

import <string>;

import vector;

export namespace tex {
struct texture {
  GLuint ID = 0;

  std::string path;
  int width = -1, height = -1, channels = -1;

  texture(const std::string &path) : path{path} {}
  ~texture() { glDeleteTextures(1, &ID); }

  void init();
};

inline namespace tex_storage {
texture font{"consolas1024.png"};
std::vector<texture *> textures{&font};

void init() {
  for (texture *tex : textures) {
    tex->init();
  }
}
} // namespace tex_storage
} // namespace tex