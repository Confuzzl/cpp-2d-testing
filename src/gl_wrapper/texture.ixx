module;

#include "util/gl.h"

export module texture;

import <string>;
import glm;
import <vector>;
import debug;

export namespace GL {
struct Texture {
  GLuint ID;
  glm::uvec2 size;

  Texture(const GLenum filter, const GLenum wrap);
  Texture(const glm::uvec2 size, const GLenum filter = GL_LINEAR,
          const GLenum wrap = GL_REPEAT);
  Texture(const std::string &name, const GLenum filter = GL_LINEAR,
          const GLenum wrap = GL_REPEAT);
  ~Texture();
  Texture(const Texture &) = delete;
  Texture(Texture &&o);
  Texture &operator=(const Texture &) = delete;
  Texture &operator=(Texture &&o);
};
} // namespace GL

export template <str NAME, GLenum filter = GL_LINEAR> GL::Texture &tex() {
  static GL::Texture out{NAME.m, filter};
  return out;
}
export GL::Texture tex(const char *name) { return {name}; }

export constexpr unsigned short TEXEL_RANGE = (1 << 15) - 1;
