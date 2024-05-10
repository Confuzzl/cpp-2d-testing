module;

#include "gl_wrapper/shaders/uniform.h"
#include <fstream>

module shaders:shader;

import debug;
import glm;

static std::string sourceToString(const std::string &name) {
  std::ifstream in{name};
  return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

using namespace shaders;

shader_t::shader_t(const GLenum type, const std::string &name)
    : type{type}, name{name} {}

void shader_t::compile() {
  const std::string source = std::format("assets/shaders/{}", name);
  GLint success = 0;
  ID = glCreateShader(type);

  const std::string temp = sourceToString(source);
  const char *chars = temp.c_str();
  glShaderSource(ID, 1, &chars, NULL);

  glCompileShader(ID);
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

  if (!success) {
    println("COMPILATION ERROR {}", source);
    return;
  }
  println("{} {}", source, ID);
}

void shader_t::cleanUp() const { glDeleteShader(ID); }