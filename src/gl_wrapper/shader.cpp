module;

#include "util/gl.h"
#include <fstream>

module shader;

import debug;
import glm;

static std::string sourceToString(const std::string &name) {
  std::ifstream in{name};
  return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

shader::base_t::base_t(const std::string &vert, const std::string &frag)
    : vert{vert}, frag{frag} {}

void shader::base_t::createShaders() {
  GLuint vertID = 0, fragID = 0;
  compileShader(GL_VERTEX_SHADER, vertID,
                std::format("assets/shaders/{}.vert", vert));
  compileShader(GL_FRAGMENT_SHADER, fragID,
                std::format("assets/shaders/{}.frag", frag));

  glAttachShader(ID, vertID);
  glAttachShader(ID, fragID);
  glLinkProgram(ID);
  glDeleteShader(vertID);
  glDeleteShader(fragID);
}

void shader::base_t::init() {
  ID = glCreateProgram();
  createShaders();
  createVAO();
  createUniforms();
}

void shader::base_t::compileShader(const GLenum type, GLuint &ID,
                                   const std::string &source) {
  GLint success = 0;
  ID = glCreateShader(type);

  std::string temp = sourceToString(source);
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

shader::font_t::font_t() : base_t("tex", "texcol") {}
void shader::font_t::createVAO() {
  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  glEnableVertexArrayAttrib(vao, 1);
  glVertexArrayAttribFormat(vao, 1, 2, GL_UNSIGNED_SHORT, false,
                            2 * sizeof(GLfloat));
  glVertexArrayAttribBinding(vao, 1, 0);
}
void shader::font_t::createUniforms() {
  view.create(ID, "view");
  color.create(ID, "color");
}
const shader::font_t &shader::font_t::setView(const glm::mat4 &matrix) const {
  setUniform(view, matrix);
  return *this;
}
const shader::font_t &
shader::font_t::setFontColor(const glm::uvec3 &col) const {
  setUniform(color, col);
  return *this;
}

shader::basic_t::basic_t() : base_t("basic", "basic") {}
void shader::basic_t::createVAO() {
  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
}
void shader::basic_t::createUniforms() {
  view.create(ID, "view");
  frag_color.create(ID, "frag_color");
}
const shader::basic_t &shader::basic_t::setView(const glm::mat4 &matrix) const {
  setUniform(view, matrix);
  return *this;
}
const shader::basic_t &
shader::basic_t::setFragColor(const glm::uvec3 &color) const {
  setUniform(frag_color, color);
  return *this;
}

shader::shape_t::shape_t() : base_t("shape", "basic") {}
void shader::shape_t::createVAO() {
  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
}
void shader::shape_t::createUniforms() {
  parent_pos.create(ID, "parent_pos");
  rotation.create(ID, "rotation");
  view.create(ID, "view");
  frag_color.create(ID, "frag_color");
}
const shader::shape_t &
shader::shape_t::setParentPos(const glm::vec2 &pos) const {
  setUniform(parent_pos, pos);
  return *this;
}
const shader::shape_t &shader::shape_t::setRotation(const float value) const {
  setUniform(rotation, value);
  return *this;
}
const shader::shape_t &shader::shape_t::setView(const glm::mat4 &matrix) const {
  setUniform(view, matrix);
  return *this;
}
const shader::shape_t &
shader::shape_t::setFragColor(const glm::uvec3 &color) const {
  setUniform(frag_color, color);
  return *this;
}

shader::circle_t::circle_t() : base_t("basic", "circle") {}
void shader::circle_t::createVAO() {
  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
}
void shader::circle_t::createUniforms() {
  center.create(ID, "center");
  radius.create(ID, "radius");
  screen_dimensions.create(ID, "screen_dimensions");
  view.create(ID, "view");
  frag_color.create(ID, "frag_color");
}
const shader::circle_t &
shader::circle_t::setCenter(const glm::vec2 &pos) const {
  setUniform(center, pos);
  return *this;
}
const shader::circle_t &shader::circle_t::setRadius(const float r) const {
  setUniform(radius, r);
  return *this;
}
const shader::circle_t &
shader::circle_t::setScreenDimensions(const glm::vec2 &dimensions) const {
  setUniform(screen_dimensions, dimensions);
  return *this;
}
const shader::circle_t &
shader::circle_t::setView(const glm::mat4 &matrix) const {
  setUniform(view, matrix);
  return *this;
}
const shader::circle_t &
shader::circle_t::setFragColor(const glm::uvec3 &color) const {
  setUniform(frag_color, color);
  return *this;
}