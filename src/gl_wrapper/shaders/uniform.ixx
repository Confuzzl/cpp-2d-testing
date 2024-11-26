module;

#include "util/gl.h"

export module uniform;

import debug;
import glm;
import gl_debug;

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
  GLuint programID;
  GLint location;

  Uniform(const GLuint programID, const char *name)
      : programID{programID}, location{glGetUniformLocation(programID, name)} {
    if (location == -1)
      // return;
      throw std::runtime_error{
          std::format("{} | {} was not a valid uniform name [{}]", programID,
                      name, glGetErrorName())};
    println("{} | {}:{}", programID, name, location);
  }
};
template <typename T> T get(const Uniform<T> &uniform);
template <std::signed_integral T> T get(const Uniform<T> &uniform) {
  GLint out;
  glGetUniformiv(uniform.programID, uniform.location, &out);
  return static_cast<T>(out);
}
template <std::unsigned_integral T> T get(const Uniform<T> &uniform) {
  GLuint out;
  glGetUniformuiv(uniform.programID, uniform.location, &out);
  return static_cast<T>(out);
}
template <> float get<float>(const Uniform<float> &uniform) {
  GLfloat out;
  glGetUniformfv(uniform.programID, uniform.location, &out);
  return out;
}
template <> double get<double>(const Uniform<double> &uniform) {
  GLdouble out;
  glGetUniformdv(uniform.programID, uniform.location, &out);
  return out;
}
template <std::size_t L, std::signed_integral T>
glm::vec<L, T> get(const Uniform<glm::vec<L, T>> &uniform) {
  glm::vec<L, T> out;
  glGetnUniformiv(uniform.programID, uniform.location, sizeof(out), &out);
  return out;
}
template <std::size_t L, std::unsigned_integral T>
glm::vec<L, T> get(const Uniform<glm::vec<L, T>> &uniform) {
  glm::vec<L, T> out;
  glGetnUniformuiv(uniform.programID, uniform.location, sizeof(out),
                   glm::value_ptr(out));
  return out;
}
template <std::size_t L>
glm::vec<L, float> get(const Uniform<glm::vec<L, float>> &uniform) {
  glm::vec<L, float> out;
  glGetnUniformfv(uniform.programID, uniform.location, sizeof(out),
                  glm::value_ptr(out));
  return out;
}
template <std::size_t L>
glm::vec<L, double> get(const Uniform<glm::vec<L, double>> &uniform) {
  glm::vec<L, double> out;
  glGetnUniformfv(uniform.programID, uniform.location, sizeof(out),
                  glm::value_ptr(out));
  return out;
}

struct Sampler {
  GLuint binding;

  static GLuint getBinding(const GLuint shaderID, const char *name) {
    const GLint location = glGetUniformLocation(shaderID, name);
    if (location == -1)
      throw std::runtime_error{
          std::format("{}: {} was not a valid sampler name [{}]", shaderID,
                      name, glGetErrorName())};
    GLuint binding;
    glGetUniformuiv(shaderID, location, &binding);
    return binding;
  }

  Sampler(const GLuint programID, const char *name)
      : binding{getBinding(programID, name)} {}
};
} // namespace shaders