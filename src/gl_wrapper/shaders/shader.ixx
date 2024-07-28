module;

#include "util/gl.h"
#include <stdexcept>

export module shaders:shader;

import vector;

import color;
import debug;
import glm;

export namespace shaders {
template <typename T>
concept has_uniform = requires(T t, const GLuint ID) {
  { T::name } -> std::convertible_to<const char *>;
  { t.createUniforms(ID) } -> std::same_as<void>;
};

template <has_uniform T> constexpr bool has_extension(const char *ext) {
  return std::string_view{T::name}.ends_with(ext);
}

struct Shader {
  GLuint ID;
  GLenum type;
  std::string name;

  Shader(const GLenum type, const std::string &name);
  Shader(const Shader &) = delete;
  Shader(Shader &&) = default;
  Shader &operator=(const Shader &) = delete;
  Shader &operator=(Shader &&) = default;

  void compile();
};
} // namespace shaders