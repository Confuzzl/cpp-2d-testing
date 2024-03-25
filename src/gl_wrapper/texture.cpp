module;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "util/gl.h"

module texture;

import <format>;
import <stdexcept>;

void tex::texture::init() {
  std::string apath = "assets/" + path;
  glCreateTextures(GL_TEXTURE_2D, 1, &ID);
  glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(apath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (not data) {
    stbi_image_free(data);
    throw std::runtime_error{std::format("TEXTURE {} FAILED TO LOAD", apath)};
  }
  glTextureStorage2D(ID, 1, GL_RGBA8, width, height);
  glTextureSubImage2D(ID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                      data);
  glGenerateTextureMipmap(ID);
  stbi_image_free(data);
}