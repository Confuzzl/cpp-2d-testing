module;

#include "util/gl.h"
#include <stdexcept>

export module shaders:shader;

import vector;

import color;
import debug;
import glm;

import buffer_objects;

namespace shaders {
template <typename T>
concept has_uniform = requires(T t, const GLuint ID) {
  { T::name } -> std::convertible_to<const char *>;
  { t.createUniforms(ID) } -> std::same_as<void>;
};

struct shader_t {
  GLenum type;
  std::string name;
  GLuint ID;

  shader_t(const GLenum type, const std::string &name);

  void compile();
  void cleanUp() const;
};
} // namespace shaders