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

using namespace shader;

shader_t::shader_t(const GLenum type, const std::string &name)
    : type{type}, name{name} {}

void shader_t::compile() {
  const std::string source = std::format("assets/shaders/{}", name);
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

void shader_t::cleanUp() { glDeleteShader(ID); }

base_t::base_t(const std::string &vert, const std::string &frag,
               std::vector<shader_t> &&otherShaders)
    : shaders{vec::New<shader_t>(shader_t{GL_VERTEX_SHADER, vert},
                                 shader_t{GL_FRAGMENT_SHADER, frag})},
      vertex{shaders[0]}, fragment{shaders[1]} {
  shaders.reserve(2 + otherShaders.size());
  shaders.insert(shaders.end(), std::make_move_iterator(otherShaders.begin()),
                 std::make_move_iterator(otherShaders.end()));
}

void base_t::createShaders() {
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

void base_t::init() {
  ID = glCreateProgram();
  createShaders();
  createVAO();
  createUniforms();
}

// void base_t::compileShader(const GLenum type, GLuint &ID,
//                            const std::string &source) {
//   GLint success = 0;
//   ID = glCreateShader(type);
//
//   std::string temp = sourceToString(source);
//   const char *chars = temp.c_str();
//   glShaderSource(ID, 1, &chars, NULL);
//
//   glCompileShader(ID);
//   glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
//
//   if (!success) {
//     println("COMPILATION ERROR {}", source);
//     return;
//   }
//   println("{} {}", source, ID);
// }

// font_t::font_t() : base_t("tex", "texcol") {}
// void font_t::createVAO() {
//   glCreateVertexArrays(1, &vao);
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
//   glVertexArrayAttribBinding(vao, 0, 0);
//   glEnableVertexArrayAttrib(vao, 1);
//   glVertexArrayAttribFormat(vao, 1, 2, GL_UNSIGNED_SHORT, false,
//                             2 * sizeof(GLfloat));
//   glVertexArrayAttribBinding(vao, 1, 0);
// }
// void font_t::createUniforms() {
//   view.create(ID, "view");
//   frag_color.create(ID, "frag_color");
// }
// const font_t &font_t::setView(const glm::mat4 &matrix) const {
//   setUniform(view, matrix);
//   return *this;
// }
// const font_t &font_t::setFontColor(const glm::uvec3 &col) const {
//   setUniform(frag_color, col);
//   return *this;
// }
//
// basic_t::basic_t() : base_t("basic", "basic") {}
// void basic_t::createVAO() {
//   glCreateVertexArrays(1, &vao);
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
//   glVertexArrayAttribBinding(vao, 0, 0);
// }
// void basic_t::createUniforms() {
//   view.create(ID, "view");
//   frag_color.create(ID, "frag_color");
// }
// const basic_t &basic_t::setView(const glm::mat4 &matrix) const {
//   setUniform(view, matrix);
//   return *this;
// }
// const basic_t &basic_t::setFragColor(const glm::uvec3 &color) const {
//   setUniform(frag_color, color);
//   return *this;
// }
//
// shape_t::shape_t() : base_t("shape", "basic") {}
// void shape_t::createVAO() {
//   glCreateVertexArrays(1, &vao);
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
//   glVertexArrayAttribBinding(vao, 0, 0);
// }
// void shape_t::createUniforms() {
//   parent_pos.create(ID, "parent_pos");
//   rotation.create(ID, "rotation");
//   view.create(ID, "view");
//   frag_color.create(ID, "frag_color");
// }
// const shape_t &shape_t::setParentPos(const glm::vec2 &pos) const {
//   setUniform(parent_pos, pos);
//   return *this;
// }
// const shape_t &shape_t::setRotation(const float value) const {
//   setUniform(rotation, value);
//   return *this;
// }
// const shape_t &shape_t::setView(const glm::mat4 &matrix) const {
//   setUniform(view, matrix);
//   return *this;
// }
// const shape_t &shape_t::setFragColor(const glm::uvec3 &color) const {
//   setUniform(frag_color, color);
//   return *this;
// }
//
// circle_t::circle_t() : base_t("basic", "circle") {}
// void circle_t::createVAO() {
//   glCreateVertexArrays(1, &vao);
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
//   glVertexArrayAttribBinding(vao, 0, 0);
// }
// void circle_t::createUniforms() {
//   center.create(ID, "center");
//   radius.create(ID, "radius");
//   screen_dimensions.create(ID, "screen_dimensions");
//   view.create(ID, "view");
//   frag_color.create(ID, "frag_color");
// }
// const circle_t &circle_t::setCenter(const glm::vec2 &pos) const {
//   setUniform(center, pos);
//   return *this;
// }
// const circle_t &circle_t::setRadius(const float r) const {
//   setUniform(radius, r);
//   return *this;
// }
// const circle_t &
// circle_t::setScreenDimensions(const glm::vec2 &dimensions) const {
//   setUniform(screen_dimensions, dimensions);
//   return *this;
// }
// const circle_t &circle_t::setView(const glm::mat4 &matrix) const {
//   setUniform(view, matrix);
//   return *this;
// }
// const circle_t &circle_t::setFragColor(const glm::uvec3 &color) const {
//   setUniform(frag_color, color);
//   return *this;
// }
//
// striped_t::striped_t() : base_t("basic", "striped") {}
// void striped_t::createVAO() {
//   glCreateVertexArrays(1, &vao);
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
//   glVertexArrayAttribBinding(vao, 0, 0);
// }
// void striped_t::createUniforms() {
//   screen_dimensions.create(ID, "screen_dimensions");
//   spacing.create(ID, "spacing");
//   frag_color.create(ID, "frag_color");
// }
// const striped_t &
// striped_t::setScreenDimensions(const glm::vec2 &dimensions) const {
//   setUniform(screen_dimensions, dimensions);
//   return *this;
// }
// const striped_t &striped_t::setSpacing(const unsigned int space) const {
//   setUniform(spacing, space);
//   return *this;
// }
// const striped_t &striped_t::setFragColor(const glm::uvec3 &color) const {
//   setUniform(frag_color, color);
//   return *this;
// }