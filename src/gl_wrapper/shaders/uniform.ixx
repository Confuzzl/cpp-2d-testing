module;

#include "util/gl.h"

export module uniform;

import debug;

export namespace shaders {
template <typename T>
concept has_uniform = requires(const GLuint ID) {
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

template <typename T> struct Uniform {
  GLint location;

  Uniform(const GLuint programID, const char *name)
      : location{glGetUniformLocation(programID, name)} {
    if (location == -1)
      throw std::runtime_error{
          std::format("{} | {} was not a valid uniform name", programID, name)};
    println("{} | {}:{}", programID, name, location);
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