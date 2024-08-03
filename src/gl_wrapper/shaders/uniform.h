#pragma once

#include "util/gl.h"
#include <format>
#include <iostream>
#include <stdexcept>

#define NEW_UNIFORM(type, name)                                                \
  Uniform<##type>##name { programID, #name }

namespace shaders {
template <typename T>
concept has_uniform = requires(T t, const GLuint ID) {
  { T::name } -> std::convertible_to<const char *>;
  T{ID};
};

template <has_uniform T> constexpr bool has_extension(const char *ext) {
  return std::string_view{T::name}.ends_with(ext);
}

struct UniformHolder {
  GLuint programID;

  UniformHolder(const GLuint programID) : programID{programID} {}
};

template <typename T = void> struct Uniform {
  GLint location;

  Uniform(const GLuint programID, const char *name)
      : location{glGetUniformLocation(programID, name)} {
    if (location == -1)
      throw std::runtime_error{
          std::format("{}: {} was not a valid uniform name", programID, name)};
    std::cout << std::format("{} | {}:{}\n", programID, name, location);
  }
};

struct Sampler {
  GLuint binding;

  static GLuint getBinding(const GLuint shaderID, const char *name) {
    const GLint location = glGetUniformLocation(shaderID, name);
    if (location == -1)
      throw std::runtime_error{
          std::format("{}: {} was not a valid sampler name", shaderID, name)};
    GLuint binding;
    glGetUniformuiv(shaderID, location, &binding);
    return binding;
  }

  Sampler(const GLuint programID, const char *name)
      : binding{getBinding(programID, name)} {}
};
} // namespace shaders