#pragma once

#include "util/gl.h"
#include <format>
#include <iostream>
#include <stdexcept>

// #define CREATE_UNIFORM(name) name.create(ID, #name)
#define NEW_UNIFORM(type, name)                                                \
  uniform<##type>##name { programID, #name }

#define SET_UNIFORM_TEMPLATE(type, call)                                       \
  template <>                                                                  \
  void setUniform<type>(const uniform<type> &uniform, const type &value)       \
      const {                                                                  \
    call;                                                                      \
  }
#define SET_SCALAR(type, scalar_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, glProgramUniform1##scalar_type(ID, uniform.location, value))
#define SET_VECTOR(type, vector_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, glProgramUniform##vector_type##v(ID, uniform.location, 1,          \
                                             glm::value_ptr(value)))
#define SET_MATRIX(type, matrix_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, glProgramUniformMatrix##matrix_type##fv(                           \
                ID, uniform.location, 1, GL_FALSE, glm::value_ptr(value)))

#define SET_UNIFORM(type, func_name, param_t, param_name, shader)              \
  type &type::set##func_name(const param_t param_name) {                       \
    setUniform(shader.param_name, param_name);                                 \
    return *this;                                                              \
  }
#define SET_UNIFORM_V(type, func_name, param_t, param_name)                    \
  SET_UNIFORM(type, func_name, param_t, param_name, vertex)
#define SET_UNIFORM_F(type, func_name, param_t, param_name)                    \
  SET_UNIFORM(type, func_name, param_t, param_name, fragment)
#define SET_UNIFORM_G(type, func_name, param_t, param_name)                    \
  SET_UNIFORM(type, func_name, param_t, param_name, geometry)

#define BIND_TEXTURE(type, sampler_name)                                       \
  type &type::bindTexture(const tex::texture &texture) {                       \
    glBindTextureUnit(fragment.sampler_name.binding, texture.ID);              \
    return *this;                                                              \
  }

namespace shaders {
template <typename T = void> struct uniform {
  GLint location;

  uniform(const GLuint shaderID, const char *name)
      : location{glGetUniformLocation(shaderID, name)} {
    if (location == -1)
      throw std::runtime_error{
          std::format("{}: {} was not a valid uniform name", shaderID, name)};
    std::cout << std::format("{} | {}:{}\n", shaderID, name, loc);
  }

  // void create(const GLuint shaderID, const std::string &name) {
  //   const GLint loc = glGetUniformLocation(shaderID, name.c_str());
  //   std::cout << std::format("{} | {}:{}\n", shaderID, name, loc);
  //   if (loc == -1)
  //     throw std::runtime_error{
  //         std::format("{}: {} was not a valid uniform name", shaderID,
  //         name)};
  //   location = loc;
  // }
};

struct sampler_t {
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

  sampler_t(const GLuint shaderID, const char *name)
      : binding{getBinding(shaderID, name)} {}

  // void create(const GLuint shaderID, const std::string &name) {
  //   const GLint loc = glGetUniformLocation(shaderID, name.c_str());
  //   if (loc == -1)
  //     throw std::runtime_error{
  //         std::format("{}: {} was not a valid sampler name", shaderID,
  //         name)};
  //   glGetUniformuiv(shaderID, loc, &binding);
  // }
};
} // namespace shaders