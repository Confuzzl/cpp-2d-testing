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
base_program_t::~base_program_t() { glDeleteProgram(ID); }

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

void base_program_t::bind(const VBOHandle &vbo) const {
  glUseProgram(ID);
  glBindVertexArray(vao);
  glVertexArrayVertexBuffer(vao, 0, vbo.vboID, vbo.offset,
                            static_cast<GLsizei>(vbo.vertexSize));
}
void base_program_t::bind(const VBOHandle &vbo, const EBOHandle &ebo) const {
  bind(vbo);
  glVertexArrayElementBuffer(vao, ebo.eboID);
}

void base_program_t::init() {
  ID = glCreateProgram();
  createShaders();
  createVAO();
  createUniforms();
}

void base_program_t::draw(const GLenum primitive, VBOHandle &vbo) const {
  bind(vbo);
  glDrawArrays(primitive, 0, vbo.count);
  vbo.reset();
}
void base_program_t::draw(const GLenum primitive, VBOHandle &vbo,
                          const EBOHandle &ebo) const {
  bind(vbo, ebo);
  glDrawElements(primitive, ebo.count, GL_UNSIGNED_INT,
                 reinterpret_cast<void *>(ebo.offset));
  vbo.reset();
}

template <>
void base_program_t::setUniform<bool>(const uniform<bool> &uniform,
                                      const bool &value) const {
  glProgramUniform1i(ID, uniform.location, value);
}
template <>
void base_program_t::setUniform<unsigned int>(
    const uniform<unsigned int> &uniform, const unsigned int &value) const {
  glProgramUniform1ui(ID, uniform.location, value);
}
template <>
void base_program_t::setUniform<float>(const uniform<float> &uniform,
                                       const float &value) const {
  glProgramUniform1f(ID, uniform.location, value);
}
template <>
void base_program_t::setUniform<glm::mat4>(const uniform<glm::mat4> &uniform,
                                           const glm::mat4 &matrix) const {
  glProgramUniformMatrix4fv(ID, uniform.location, 1, GL_FALSE,
                            glm::value_ptr(matrix));
}
template <>
void base_program_t::setUniform<glm::vec2>(const uniform<glm::vec2> &uniform,
                                           const glm::vec2 &vector) const {
  glProgramUniform2fv(ID, uniform.location, 1, glm::value_ptr(vector));
}
template <>
void base_program_t::setUniform<glm::uvec2>(const uniform<glm::uvec2> &uniform,
                                            const glm::uvec2 &vector) const {
  glProgramUniform2uiv(ID, uniform.location, 1, glm::value_ptr(vector));
}
template <>
void base_program_t::setUniform<glm::uvec3>(const uniform<glm::uvec3> &uniform,
                                            const glm::uvec3 &vector) const {
  glProgramUniform3uiv(ID, uniform.location, 1, glm::value_ptr(vector));
}

SET_UNIFORM(texcol_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(texcol_t, FragColor, color_t &, frag_color, fragment)
BIND_TEXTURE(texcol_t, sampler);

SET_UNIFORM(sdf_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(sdf_t, FragColor, color_t &, frag_color, fragment)
SET_UNIFORM(sdf_t, Threshold, float, threshold, fragment)
SET_UNIFORM(sdf_t, FontSize, float, font_size, fragment)
SET_UNIFORM(sdf_t, AntiAlias, bool, anti_alias, fragment)
BIND_TEXTURE(sdf_t, sampler);

SET_UNIFORM(basic_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(basic_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(trans_t, ParentPos, glm::vec2, parent_pos, vertex)
SET_UNIFORM(trans_t, Rotation, float, rotation, vertex)
SET_UNIFORM(trans_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(trans_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(striped_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(striped_t, Width, unsigned int, width, fragment)
SET_UNIFORM(striped_t, Spacing, unsigned int, spacing, fragment)
striped_t &striped_t::setPattern(const Pattern pattern) {
  setUniform(fragment.pattern, static_cast<unsigned int>(pattern));
  return *this;
}
SET_UNIFORM(striped_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(line_t, View, glm::mat4 &, view, geometry)
SET_UNIFORM(line_t, Thickness, float, thickness, geometry)
SET_UNIFORM(line_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(circ_t, View, glm::mat4 &, view, geometry)
SET_UNIFORM(circ_t, Radius, float, radius, geometry)
SET_UNIFORM(circ_t, Center, glm::vec2, center, fragment)
SET_UNIFORM(circ_t, ScreenDimensions, glm::uvec2, screen_dimensions, fragment)
SET_UNIFORM(circ_t, FragColor, color_t &, frag_color, fragment)