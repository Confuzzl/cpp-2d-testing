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
  std::vector<shader_t> shaders;
  shader_t &vertex, &fragment;

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

  simple_program_t() : base_program_t(V::name, F::name) {}

  void createVAO() override { vertex.createVAO(vao); }
  void createUniforms() override {
    vertex.createUniforms(ID);
    fragment.createUniforms(ID);
  }
};

struct font_t : simple_program_t<vert::tex, frag::texcol> {
  font_t &setView(const glm::mat4 &mat) {
    setUniform(vertex.view, mat);
    return *this;
  }
  font_t &setFragColor(const color_t &frag_color) {
    setUniform(fragment.frag_color, frag_color);
    return *this;
  }
};
struct basic_t : simple_program_t<vert::basic, frag::basic> {
  basic_t &setView(const glm::mat4 &view) {
    setUniform(vertex.view, view);
    return *this;
  }
  basic_t &setFragColor(const color_t &frag_color) {
    setUniform(fragment.frag_color, frag_color);
    return *this;
  }
};
struct trans_t : simple_program_t<vert::trans, frag::basic> {
  trans_t &setParentPos(const glm::vec2 &pos) {
    setUniform(vertex.parent_pos, pos);
    return *this;
  }
  trans_t &setRotation(const float rotation) {
    setUniform(vertex.rotation, rotation);
    return *this;
  }
  trans_t &setView(const glm::mat4 &view) {
    setUniform(vertex.view, view);
    return *this;
  }
  trans_t &setFragColor(const color_t &frag_color) {
    setUniform(fragment.frag_color, frag_color);
    return *this;
  }
};
struct circle_t : simple_program_t<vert::basic, frag::circle> {
  circle_t &setView(const glm::mat4 &view) {
    setUniform(vertex.view, view);
    return *this;
  }
  circle_t &setCenter(const glm::vec2 &center) {
    setUniform(fragment.center, center);
    return *this;
  }
  circle_t &setRadius(const float radius) {
    setUniform(fragment.radius, radius);
    return *this;
  }
  circle_t &setScreenDimensions(const glm::uvec2 &screen_dimensions) {
    setUniform(fragment.screen_dimensions, screen_dimensions);
    return *this;
  }
  circle_t &setFragColor(const color_t &frag_color) {
    setUniform(fragment.frag_color, frag_color);
    return *this;
  }
};
struct striped_t : simple_program_t<vert::basic, frag::striped> {
  striped_t &setView(const glm::mat4 &view) {
    setUniform(vertex.view, view);
    return *this;
  }
  striped_t &setWidth(const unsigned int spacing) {
    setUniform(fragment.spacing, spacing);
    return *this;
  }
  striped_t &setSpacing(const unsigned int spacing) {
    setUniform(fragment.spacing, spacing);
    return *this;
  }
  enum struct Pattern { FORWARD = 1, BACKWARD = 2, CROSS = 3 };
  striped_t &setSpacing(const Pattern pattern) {
    setUniform(fragment.pattern, static_cast<unsigned int>(pattern));
    return *this;
  }
  striped_t &setFragColor(const color_t &frag_color) {
    setUniform(fragment.frag_color, frag_color);
    return *this;
  }
};
} // namespace shaders