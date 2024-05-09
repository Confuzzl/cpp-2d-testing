module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:program;

import debug;

using namespace shaders;

base_program_t::base_program_t(const std::string &vert, const std::string &frag,
                               std::vector<shader_t> &&otherShaders)
    : shaders{vec::New<shader_t>(shader_t{GL_VERTEX_SHADER, vert},
                                 shader_t{GL_FRAGMENT_SHADER, frag})},
      vertex{shaders[0]}, fragment{shaders[1]} {
  shaders.reserve(2 + otherShaders.size());
  shaders.insert(shaders.end(), std::make_move_iterator(otherShaders.begin()),
                 std::make_move_iterator(otherShaders.end()));
}

void base_program_t::createShaders() {
  println("PROGRAM: {}", ID);
  for (shader_t &shader : shaders) {
    shader.compile();
    glAttachShader(ID, shader.ID);
  }
  glLinkProgram(ID);
  for (shader_t &shader : shaders) {
    shader.cleanUp();
  }
}

void base_program_t::init() {
  ID = glCreateProgram();
  createShaders();
  createVAO();
  createUniforms();
}

void base_program_t::use() const {
  glUseProgram(ID);
  glBindVertexArray(vao);
}

template <>
void base_program_t::setUniform<unsigned int>(
    const uniform<unsigned int> &uniform, const unsigned int &value) const {
  glUniform1ui(uniform.location, value);
}
template <>
void base_program_t::setUniform<float>(const uniform<float> &uniform,
                                       const float &value) const {
  glUniform1f(uniform.location, value);
}
template <>
void base_program_t::setUniform<glm::mat4>(const uniform<glm::mat4> &uniform,
                                           const glm::mat4 &matrix) const {
  glUniformMatrix4fv(uniform.location, 1, GL_FALSE, glm::value_ptr(matrix));
}
template <>
void base_program_t::setUniform<glm::vec2>(const uniform<glm::vec2> &uniform,
                                           const glm::vec2 &vector) const {
  glUniform2fv(uniform.location, 1, glm::value_ptr(vector));
}
template <>
void base_program_t::setUniform<glm::uvec2>(const uniform<glm::uvec2> &uniform,
                                            const glm::uvec2 &vector) const {
  glUniform2uiv(uniform.location, 1, glm::value_ptr(vector));
}
template <>
void base_program_t::setUniform<glm::uvec3>(const uniform<glm::uvec3> &uniform,
                                            const glm::uvec3 &vector) const {
  glUniform3uiv(uniform.location, 1, glm::value_ptr(vector));
}