module;

#include "gl_wrapper/shaders/uniform.h"
#include <fstream>
#include <stdexcept>

module shaders:shader;

import debug;
import glm;

static std::string sourceToString(const std::string &name) {
  std::ifstream in{name};
  return {std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};
}

using namespace shaders;

Shader::Shader(const GLenum type, const char *name)
    : ID{glCreateShader(type)}, type{type}, name{name} {
  const std::string temp =
      sourceToString(std::format("assets/shaders/{}", name));
  const char *chars = temp.c_str();
  glShaderSource(ID, 1, &chars, nullptr);

  glCompileShader(ID);

  GLint success = 0;
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
  if (!success)
    throw std::runtime_error{std::format("COMPILATION ERROR {}", name)};
}
Shader::~Shader() { glDeleteShader(ID); }
