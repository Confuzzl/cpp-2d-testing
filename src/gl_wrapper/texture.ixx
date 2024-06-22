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
std::vector<texture> textures{
    vec::New<texture>("consolas1024.png", "sdf1024.png")};
texture &font = textures[0];
texture &sdfFont = textures[1];

void init() {
  // for (texture *tex : textures) {
  //   tex->init();
  // }
  for (texture &tex : textures) {
    tex.init();
  }
}
} // namespace tex_storage
} // namespace tex