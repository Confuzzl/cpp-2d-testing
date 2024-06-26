module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:program;

import vector;
import texture;
import glm;
import color;

import :shader;
import :vertex;
import :fragment;
import :geometry;

import buffer_object;

export namespace shaders {
// merge w simple
struct base_program_t {
  GLuint ID;
  GLuint vao;

  ~base_program_t();

protected:
  base_program_t(const std::string &vert, const std::string &frag,
                 std::vector<shader_t> &&otherShaders = {});

private:
  std::vector<shader_t> compileList;

  void createShaders();
  virtual void createVAO() = 0;
  virtual void createUniforms() = 0;

  void bind(const VBOHandle &vbo) const;
  void bind(const VBOHandle &vbo, const EBOHandle &ebo) const;

public:
  void init();

  void draw(const GLenum primitive, VBOHandle &vbo) const;
  void draw(const GLenum primitive, VBOHandle &vbo, const EBOHandle &ebo) const;

  template <typename T>
  void setUniform(const uniform<T> &uniform, const T &value) const;

  template <>
  void setUniform<bool>(const uniform<bool> &uniform, const bool &value) const;
  template <>
  void setUniform<unsigned int>(const uniform<unsigned int> &uniform,
                                const unsigned int &value) const;
  template <>
  void setUniform<float>(const uniform<float> &uniform,
                         const float &value) const;
  template <>
  void setUniform<glm::mat4>(const uniform<glm::mat4> &uniform,
                             const glm::mat4 &matrix) const;
  template <>
  void setUniform<glm::vec2>(const uniform<glm::vec2> &uniform,
                             const glm::vec2 &vector) const;
  template <>
  void setUniform<glm::uvec2>(const uniform<glm::uvec2> &uniform,
                              const glm::uvec2 &vector) const;
  template <>
  void setUniform<glm::uvec3>(const uniform<glm::uvec3> &uniform,
                              const glm::uvec3 &vector) const;
};

template <vert::format V, has_uniform F>
struct simple_program_t : base_program_t {
  V vertex;
  F fragment;

  simple_program_t(std::vector<shader_t> &&otherShaders = {})
      : base_program_t(V::name, F::name, std::move(otherShaders)) {}

  void createVAO() override { vertex.createVAO(vao); }
  void createUniforms() override {
    vertex.createUniforms(ID);
    fragment.createUniforms(ID);
  }
};

struct texcol_t : simple_program_t<vert::tex, frag::texcol> {
  texcol_t &setView(const glm::mat4 &view);
  texcol_t &setFragColor(const color_t &frag_color);
  texcol_t &bindTexture(const tex::texture &texture);
};

struct sdf_t : simple_program_t<vert::tex, frag::sdf_font> {
  sdf_t &setView(const glm::mat4 &view);
  sdf_t &setFragColor(const color_t &frag_color);
  sdf_t &setThreshold(const float threshold);
  sdf_t &setFontSize(const float font_size);
  sdf_t &setAntiAlias(const bool anti_alias);
  sdf_t &bindTexture(const tex::texture &texture);
};
struct basic_t : simple_program_t<vert::basic, frag::basic> {
  basic_t &setView(const glm::mat4 &view);
  basic_t &setFragColor(const color_t &frag_color);
};
struct trans_t : simple_program_t<vert::trans, frag::basic> {
  trans_t &setParentPos(const glm::vec2 parent_pos);
  trans_t &setRotation(const float rotation);
  trans_t &setView(const glm::mat4 &view);
  trans_t &setFragColor(const color_t &frag_color);
};
struct striped_t : simple_program_t<vert::basic, frag::striped> {
  striped_t &setView(const glm::mat4 &view);
  striped_t &setWidth(const unsigned int width);
  striped_t &setSpacing(const unsigned int spacing);
  enum struct Pattern { FORWARD = 1, BACKWARD = 2, CROSS = 3 };
  striped_t &setPattern(const Pattern pattern);
  striped_t &setFragColor(const color_t &frag_color);
};

template <vert::format V, has_uniform F, has_uniform G>
struct geometry_program_t : simple_program_t<V, F> {
  G geometry;

  geometry_program_t()
      : simple_program_t<V, F>({{GL_GEOMETRY_SHADER, G::name}}) {}

  void createUniforms() override {
    simple_program_t<V, F>::createUniforms();
    geometry.createUniforms(base_program_t::ID);
  }
};

struct line_t : geometry_program_t<vert::identity, frag::basic, geom::line> {
  line_t &setView(const glm::mat4 &view);
  line_t &setThickness(const float thickness);
  line_t &setFragColor(const color_t &frag_color);
};

struct circ_t : geometry_program_t<vert::identity, frag::circle, geom::circle> {
  circ_t &setView(const glm::mat4 &view);
  circ_t &setRadius(const float radius);
  circ_t &setCenter(const glm::vec2 center);
  circ_t &setScreenDimensions(const glm::uvec2 screen_dimensions);
  circ_t &setFragColor(const color_t &frag_color);
};
} // namespace shaders