module;

#include "util/gl.h"
#include <stdexcept>

export module shaders:shader;

import vector;

import color;
import debug;
import glm;

export namespace GL {
struct Shader {
  GLuint ID;
  GLenum type;
  const char *name;

  Shader(const GLenum type, const char *name);
  ~Shader();
  Shader(const Shader &) = delete;
  Shader(Shader &&o);
  Shader &operator=(const Shader &) = delete;
  Shader &operator=(Shader &&o);
};
} // namespace GL