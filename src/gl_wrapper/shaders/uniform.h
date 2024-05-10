#pragma once

#include "util/gl.h"
#include <format>
#include <stdexcept>

#define CREATE_UNIFORM(name) name.create(ID, #name)

namespace shaders {
template <typename T> struct uniform {
  GLint location;

  void create(GLuint shaderID, const std::string &name) {
    const GLint loc = glGetUniformLocation(shaderID, name.c_str());
    if (loc == -1)
      throw std::runtime_error{
          std::format("{}: {} was not a valid uniform name", shaderID, name)};
    location = loc;
  }
};
} // namespace shaders