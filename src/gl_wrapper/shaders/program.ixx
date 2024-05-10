module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:program;

import vector;
import buffer_objects;
import glm;

import :shader;
import :vertex;
import :fragment;
import :geometry;

export namespace shaders {
struct base_program_t {
  GLuint ID;
  GLuint vao;

protected:
  base_program_t(const std::string &vert, const std::string &frag,
                 std::vector<shader_t> &&otherShaders = {});

private:
  std::vector<shader_t> compileList;

  void createShaders();
  virtual void createVAO() = 0;
  virtual void createUniforms() = 0;

public:
  void init();

  void use() const;
  template <typename T> void use(const vbo<T> &vbo) const {
    use();
    glVertexArrayVertexBuffer(vao, 0, vbo.ID, 0, sizeof(T));
  }
  template <typename T, typename E>
  void use(const vbo<T> &vbo, const E &ebo) const {
    use(vbo);
    glVertexArrayElementBuffer(vao, ebo.ID);
  }

  template <typename T>
  void setUniform(const uniform<T> &uniform, const T &value) const;
  template <>
  void setUniform<unsigned int>(const uniform<unsigned int> &uniform,
                                const unsigned int &value) const {
    glUniform1ui(uniform.location, value);
  }
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

struct font_t : simple_program_t<vert::tex, frag::texcol> {
  font_t &setView(const glm::mat4 &mat);
  font_t &setFragColor(const color_t &frag_color);
};
struct basic_t : simple_program_t<vert::basic, frag::basic> {
  basic_t &setView(const glm::mat4 &view);
  basic_t &setFragColor(const color_t &frag_color);
};
struct trans_t : simple_program_t<vert::trans, frag::basic> {
  trans_t &setParentPos(const glm::vec2 &pos);
  trans_t &setRotation(const float rotation);
  trans_t &setView(const glm::mat4 &view);
  trans_t &setFragColor(const color_t &frag_color);
};
struct circle_t : simple_program_t<vert::basic, frag::circle> {
  circle_t &setView(const glm::mat4 &view);
  circle_t &setCenter(const glm::vec2 &center);
  circle_t &setRadius(const float radius);
  circle_t &setScreenDimensions(const glm::uvec2 &screen_dimensions);
  circle_t &setFragColor(const color_t &frag_color);
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

struct line_t : geometry_program_t<vert::basic, frag::basic, geom::line> {
  line_t &setView(const glm::mat4 &view);
  line_t &setFragColor(const color_t &frag_color);
  line_t &setThickness(const float thickness);
};
} // namespace shaders