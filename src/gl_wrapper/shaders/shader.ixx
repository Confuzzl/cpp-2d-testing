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
  { T::createUniforms(ID) } -> std::same_as<void>;
};

template <std::size_t L> constexpr const char *extension(const char (&str)[L]) {
  return str + (L - 5);
}
template <has_uniform T> constexpr bool has_extension(const char *ext) {
  return std::string_view{extension(T::name)} == ext;
}

struct shader_t {
  GLuint ID;
  GLenum type;
  std::string name;

  shader_t(const GLenum type, const std::string &name);
  shader_t(const shader_t &) = delete;
  shader_t(shader_t &&) = default;
  shader_t &operator=(const shader_t &) = delete;
  shader_t &operator=(shader_t &&) = default;

  void compile();
};
} // namespace shaders