module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:program;

import debug;

using namespace shaders;

base_program_t::base_program_t(const std::string &vert, const std::string &frag,
                               std::vector<shader_t> &&otherShaders) {
  compileList.reserve(2 + otherShaders.size());
  compileList.emplace_back(GL_VERTEX_SHADER, vert);
  compileList.emplace_back(GL_FRAGMENT_SHADER, frag);
  for (shader_t &shader : otherShaders)
    compileList.emplace_back(std::move(shader));
}
base_program_t::~base_program_t() { glDeleteShader(ID); }

void base_program_t::createShaders() {
  println("PROGRAM: {}", ID);
  for (shader_t &shader : compileList) {
    shader.compile();
    glAttachShader(ID, shader.ID);
  }
  glLinkProgram(ID);
  for (shader_t &shader : compileList) {
    glDetachShader(ID, shader.ID);
    glDeleteShader(shader.ID);
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
void base_program_t::setUniform<bool>(const uniform<bool> &uniform,
                                      const bool &value) const {
  glUniform1i(uniform.location, value);
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

font_t &font_t::setView(const glm::mat4 &mat) {
  setUniform(vertex.view, mat);
  return *this;
}
font_t &font_t::setFragColor(const color_t &frag_color) {
  setUniform(fragment.frag_color, frag_color);
  return *this;
}

basic_t &basic_t::setView(const glm::mat4 &view) {
  setUniform(vertex.view, view);
  return *this;
}
basic_t &basic_t::setFragColor(const color_t &frag_color) {
  setUniform(fragment.frag_color, frag_color);
  return *this;
}

trans_t &trans_t::setParentPos(const glm::vec2 &pos) {
  setUniform(vertex.parent_pos, pos);
  return *this;
}
trans_t &trans_t::setRotation(const float rotation) {
  setUniform(vertex.rotation, rotation);
  return *this;
}
trans_t &trans_t::setView(const glm::mat4 &view) {
  setUniform(vertex.view, view);
  return *this;
}
trans_t &trans_t::setFragColor(const color_t &frag_color) {
  setUniform(fragment.frag_color, frag_color);
  return *this;
}

circle_t &circle_t::setView(const glm::mat4 &view) {
  setUniform(vertex.view, view);
  return *this;
}
circle_t &circle_t::setCenter(const glm::vec2 &center) {
  setUniform(fragment.center, center);
  return *this;
}
circle_t &circle_t::setRadius(const float radius) {
  setUniform(fragment.radius, radius);
  return *this;
}
circle_t &circle_t::setScreenDimensions(const glm::uvec2 &screen_dimensions) {
  setUniform(fragment.screen_dimensions, screen_dimensions);
  return *this;
}
circle_t &circle_t::setFragColor(const color_t &frag_color) {
  setUniform(fragment.frag_color, frag_color);
  return *this;
}

striped_t &striped_t::setView(const glm::mat4 &view) {
  setUniform(vertex.view, view);
  return *this;
}
striped_t &striped_t::setWidth(const unsigned int width) {
  setUniform(fragment.width, width);
  return *this;
}
striped_t &striped_t::setSpacing(const unsigned int spacing) {
  setUniform(fragment.spacing, spacing);
  return *this;
}
striped_t &striped_t::setPattern(const Pattern pattern) {
  setUniform(fragment.pattern, static_cast<unsigned int>(pattern));
  return *this;
}
striped_t &striped_t::setFragColor(const color_t &frag_color) {
  setUniform(fragment.frag_color, frag_color);
  return *this;
}

line_t &line_t::setView(const glm::mat4 &view) {
  setUniform(geometry.view, view);
  return *this;
}
line_t &line_t::setThickness(const float thickness) {
  setUniform(geometry.thickness, thickness);
  return *this;
}
line_t &line_t::setFragColor(const color_t &frag_color) {
  setUniform(fragment.frag_color, frag_color);
  return *this;
}