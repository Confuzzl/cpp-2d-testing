module;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "util/gl.h"
#include <stdexcept>

module texture;

import <format>;

using namespace GL;

Texture::Texture(const std::string &name) {
  std::string path = "assets/" + name;

  glCreateTextures(GL_TEXTURE_2D, 1, &ID);
  glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  int width, height, channels;
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (!data) {
    stbi_image_free(data);
    throw std::runtime_error{std::format("TEXTURE {} FAILED TO LOAD", path)};
  }
  glTextureStorage2D(ID, 1, GL_RGBA8, width, height);
  glTextureSubImage2D(ID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                      data);
  glGenerateTextureMipmap(ID);
  stbi_image_free(data);
}
Texture::~Texture() {
  if (ID)
    glDeleteTextures(1, &ID);
}
Texture::Texture(Texture &&o) : ID{o.ID} { o.ID = 0; }
Texture &Texture::operator=(Texture &&o) {
  ID = o.ID;
  o.ID = 0;
  return *this;
}
