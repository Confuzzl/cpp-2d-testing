module;

#include "util/gl.h"
#include <fstream>
#include <stdexcept>

module shaders:shader;

import debug;
import glm;

static std::string sourceToString(const std::string &name) {
  std::ifstream in{name};
  return {std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};
}

using namespace GL;

Shader::Shader(const GLenum type, const char *name)
    : ID{glCreateShader(type)}, type{type}, name{name} {
  const std::string temp =
      sourceToString(std::format("assets/shaders/{}", name));
  const char *chars = temp.c_str();
  glShaderSource(ID, 1, &chars, nullptr);

  glCompileShader(ID);

  GLint success = 0;
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint size = 0;
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &size);
    std::string log;
    log.resize(size);
    glGetShaderInfoLog(ID, size, &size, &log[0]);
    throw std::runtime_error{
        std::format("COMPILATION ERROR {}\n{}", name, log)};
  }
  println("Successfully compiled {}", name);
}
Shader::~Shader() {
  if (name)
    glDeleteShader(ID);
}
Shader::Shader(Shader &&o) : ID{o.ID}, type{o.type}, name{o.name} { o.ID = 0; }
Shader &Shader::operator=(Shader &&o) {
  ID = o.ID;
  type = o.type;
  name = o.name;
  o.ID = 0;
  return *this;
}
