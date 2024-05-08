module;

#include "util/gl.h"

#define CREATE_UNIFORM(name) name.create(ID, #name)

export module shader;

import <stdexcept>;

import vector;

import color;
import debug;
import glm;

import buffer_objects;

template <typename T> struct uniform {
  GLint location;

  void create(GLuint shaderID, const std::string &name) {
    const GLint loc = glGetUniformLocation(shaderID, name.c_str());
    if (loc == -1)
      throw std::runtime_error{
          std::format("{} was not a valid uniform name", name)};
    location = loc;
  }
};

export namespace shader {
template <typename T>
concept has_uniform = requires(T t, const GLuint ID) {
  { T::name } -> std::convertible_to<const char *>;
  { t.createUniforms(ID) } -> std::same_as<void>;
};

namespace vert {
template <typename T>
concept format = has_uniform<T> && requires(T t, GLuint &vao) {
  { t.createVAO(vao) } -> std::same_as<void>;
};

struct basic {
  static constexpr char name[] = "basic.vert";

  uniform<glm::mat4> view;

  void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
  }

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }
};

struct trans {
  static constexpr char name[] = "trans.vert";

  uniform<glm::vec2> parent_pos;
  uniform<float> rotation;
  uniform<glm::mat4> view;

  void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
  }

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(parent_pos);
    CREATE_UNIFORM(rotation);
    CREATE_UNIFORM(view);
  }
};
struct tex {
  static constexpr char name[] = "tex.vert";

  uniform<glm::mat4> view;

  void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 2, GL_UNSIGNED_SHORT, false,
                              2 * sizeof(GLfloat));
    glVertexArrayAttribBinding(vao, 1, 0);
  }

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }
};
} // namespace vert

namespace frag {
struct basic {
  static constexpr char name[] = "basic.frag";

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(frag_color); }
};
struct circle {
  static constexpr char name[] = "circle.frag";

  uniform<glm::vec2> center;
  uniform<float> radius;

  uniform<glm::uvec2> screen_dimensions;
  uniform<glm::mat4> view;

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(center);
    CREATE_UNIFORM(radius);
    CREATE_UNIFORM(screen_dimensions);
    CREATE_UNIFORM(view);
    CREATE_UNIFORM(frag_color);
  }
};
struct striped {
  static constexpr char name[] = "striped.frag";

  uniform<unsigned int> width;
  uniform<unsigned int> spacing;
  uniform<unsigned int> pattern;

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(width);
    CREATE_UNIFORM(spacing);
    CREATE_UNIFORM(pattern);
    CREATE_UNIFORM(frag_color);
  }
};
struct texcol {
  static constexpr char name[] = "texcol.frag";

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(frag_color); }
};
} // namespace frag

namespace geom {
struct line {
  uniform<float> thickness;

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(thickness); }
};
} // namespace geom

struct shader_t {
  GLenum type;
  std::string name;
  GLuint ID;

  shader_t(const GLenum type, const std::string &name);

  void compile();
  void cleanUp();
};

struct base_t {
  GLuint ID;
  GLuint vao;

protected:
  base_t(const std::string &vert, const std::string &frag,
         std::vector<shader_t> &&otherShaders = {});

private:
  std::vector<shader_t> shaders;
  shader_t &vertex, &fragment;

  void createShaders();
  // void compileShader(const GLenum type, GLuint &ID, const std::string
  // &source);
  virtual void createVAO() = 0;
  virtual void createUniforms() = 0;

public:
  void init();

  void use() const {
    glUseProgram(ID);
    glBindVertexArray(vao);
  }
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
                         const float &value) const {
    glUniform1f(uniform.location, value);
  }
  template <>
  void setUniform<glm::mat4>(const uniform<glm::mat4> &uniform,
                             const glm::mat4 &matrix) const {
    glUniformMatrix4fv(uniform.location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
  template <>
  void setUniform<glm::vec2>(const uniform<glm::vec2> &uniform,
                             const glm::vec2 &vector) const {
    glUniform2fv(uniform.location, 1, glm::value_ptr(vector));
  }
  template <>
  void setUniform<glm::uvec2>(const uniform<glm::uvec2> &uniform,
                              const glm::uvec2 &vector) const {
    glUniform2uiv(uniform.location, 1, glm::value_ptr(vector));
  }
  template <>
  void setUniform<glm::uvec3>(const uniform<glm::uvec3> &uniform,
                              const glm::uvec3 &vector) const {
    glUniform3uiv(uniform.location, 1, glm::value_ptr(vector));
  }
};

template <vert::format V, has_uniform F> struct program_t : base_t {
  V vertex;
  F fragment;

  program_t() : base_t(V::name, F::name) {}

  void createVAO() override { vertex.createVAO(vao); }
  void createUniforms() override {
    vertex.createUniforms(ID);
    fragment.createUniforms(ID);
  }
};

struct font_t : program_t<vert::tex, frag::texcol> {
  font_t &setView(const glm::mat4 &mat) {
    setUniform(vertex.view, mat);
    return *this;
  }
  font_t &setFragColor(const color_t &frag_color) {
    setUniform(fragment.frag_color, frag_color);
    return *this;
  }
};
struct basic_t : program_t<vert::basic, frag::basic> {
  basic_t &setView(const glm::mat4 &view) {
    setUniform(vertex.view, view);
    return *this;
  }
  basic_t &setFragColor(const color_t &frag_color) {
    setUniform(fragment.frag_color, frag_color);
    return *this;
  }
};
struct trans_t : program_t<vert::trans, frag::basic> {
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
struct circle_t : program_t<vert::basic, frag::circle> {
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
struct striped_t : program_t<vert::basic, frag::striped> {
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

// export struct font_t : public base_t {
//   uniform<glm::mat4> view;
//   uniform<glm::uvec3> frag_color;
//
//   font_t();
//
// private:
//   void createVAO() override;
//   void createUniforms() override;
//
// public:
//   const font_t &setView(const glm::mat4 &matrix) const;
//   const font_t &setFontColor(const glm::uvec3 &col) const;
// };
//
// export struct basic_t : public base_t {
//   uniform<glm::mat4> view;
//   uniform<glm::uvec3> frag_color;
//
//   basic_t();
//
// private:
//   void createVAO() override;
//   void createUniforms() override;
//
// public:
//   const basic_t &setView(const glm::mat4 &matrix) const;
//   const basic_t &setFragColor(const glm::uvec3 &color) const;
// };
// using ui_t = basic_t;
//
// export struct shape_t : public base_t {
//   uniform<glm::vec2> parent_pos;
//   uniform<float> rotation;
//   uniform<glm::mat4> view;
//   uniform<glm::uvec3> frag_color;
//
//   shape_t();
//
// private:
//   void createVAO() override;
//   void createUniforms() override;
//
// public:
//   const shape_t &setParentPos(const glm::vec2 &pos) const;
//   const shape_t &setRotation(const float value) const;
//   const shape_t &setView(const glm::mat4 &matrix) const;
//   const shape_t &setFragColor(const glm::uvec3 &color) const;
// };
//
// export struct circle_t : public base_t {
//   uniform<glm::vec2> center;
//   uniform<float> radius;
//
//   uniform<glm::vec2> screen_dimensions;
//   uniform<glm::mat4> view;
//
//   uniform<glm::uvec3> frag_color;
//
//   circle_t();
//
// private:
//   void createVAO() override;
//   void createUniforms() override;
//
// public:
//   const circle_t &setCenter(const glm::vec2 &pos) const;
//   const circle_t &setRadius(const float r) const;
//   const circle_t &setScreenDimensions(const glm::vec2 &dimensions) const;
//   const circle_t &setView(const glm::mat4 &matrix) const;
//   const circle_t &setFragColor(const glm::uvec3 &color) const;
// };
//
// export struct striped_t : public base_t {
//   uniform<glm::vec2> screen_dimensions;
//   uniform<unsigned int> spacing;
//
//   uniform<glm::uvec3> frag_color;
//
//   striped_t();
//
// private:
//   void createVAO() override;
//   void createUniforms() override;
//
// public:
//   const striped_t &setScreenDimensions(const glm::vec2 &dimensions) const;
//   const striped_t &setSpacing(const unsigned int space) const;
//   const striped_t &setFragColor(const glm::uvec3 &color) const;
// };

inline namespace holder {
shader::font_t font{};
shader::basic_t basic{};
shader::trans_t shape{};
shader::circle_t circle{};
shader::striped_t striped{};

void init() {
  font.init();
  basic.init();
  shape.init();
  circle.init();
  striped.init();
}
} // namespace holder
} // namespace shader