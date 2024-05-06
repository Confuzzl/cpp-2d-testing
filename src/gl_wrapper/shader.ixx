module;

#include "util/gl.h"

export module shader;

import <stdexcept>;

import vector;

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
namespace vert {
struct basic {
  static constexpr char name[] = "basic.vert";
  static void createVAO(GLuint &vao);
};
struct shape {
  static constexpr char name[] = "shape.vert";
  static void createVAO(GLuint &vao);
};
struct tex {
  static constexpr char name[] = "tex.vert";
  static void createVAO(GLuint &vao);
};
} // namespace vert
namespace frag {
struct basic {
  static constexpr char name[] = "basic.frag";
};
struct circle {
  static constexpr char name[] = "circle.frag";
};
struct striped {
  static constexpr char name[] = "striped.frag";
};
struct texcol {
  static constexpr char name[] = "texcol.frag";
};
} // namespace frag

template <typename V, typename F> struct foo_t {
  using vertex = V;
  using fragment = F;
};

struct base_t {
  GLuint ID;
  GLuint vao;

protected:
  base_t(const std::string &vert, const std::string &frag);

private:
  std::string vert, frag;

  void createShaders();
  void compileShader(const GLenum type, GLuint &ID, const std::string &source);
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
  void setUniform<glm::uvec3>(const uniform<glm::uvec3> &uniform,
                              const glm::uvec3 &vector) const {
    glUniform3uiv(uniform.location, 1, glm::value_ptr(vector));
  }
};

export struct font_t : public base_t {
  uniform<glm::mat4> view;
  uniform<glm::uvec3> color;

  font_t();

private:
  void createVAO() override;
  void createUniforms() override;

public:
  const font_t &setView(const glm::mat4 &matrix) const;
  const font_t &setFontColor(const glm::uvec3 &col) const;
};

export struct basic_t : public base_t {
  uniform<glm::mat4> view;
  uniform<glm::uvec3> frag_color;

  basic_t();

private:
  void createVAO() override;
  void createUniforms() override;

public:
  const basic_t &setView(const glm::mat4 &matrix) const;
  const basic_t &setFragColor(const glm::uvec3 &color) const;
};
using ui_t = basic_t;

export struct shape_t : public base_t {
  uniform<glm::vec2> parent_pos;
  uniform<float> rotation;
  uniform<glm::mat4> view;
  uniform<glm::uvec3> frag_color;

  shape_t();

private:
  void createVAO() override;
  void createUniforms() override;

public:
  const shape_t &setParentPos(const glm::vec2 &pos) const;
  const shape_t &setRotation(const float value) const;
  const shape_t &setView(const glm::mat4 &matrix) const;
  const shape_t &setFragColor(const glm::uvec3 &color) const;
};

export struct circle_t : public base_t {
  uniform<glm::vec2> center;
  uniform<float> radius;

  uniform<glm::vec2> screen_dimensions;
  uniform<glm::mat4> view;

  uniform<glm::uvec3> frag_color;

  circle_t();

private:
  void createVAO() override;
  void createUniforms() override;

public:
  const circle_t &setCenter(const glm::vec2 &pos) const;
  const circle_t &setRadius(const float r) const;
  const circle_t &setScreenDimensions(const glm::vec2 &dimensions) const;
  const circle_t &setView(const glm::mat4 &matrix) const;
  const circle_t &setFragColor(const glm::uvec3 &color) const;
};

export struct striped_t : public base_t {
  uniform<glm::vec2> screen_dimensions;
  uniform<unsigned int> spacing;

  uniform<glm::uvec3> frag_color;

  striped_t();

private:
  void createVAO() override;
  void createUniforms() override;

public:
  const striped_t &setScreenDimensions(const glm::vec2 &dimensions) const;
  const striped_t &setSpacing(const unsigned int space) const;
  const striped_t &setFragColor(const glm::uvec3 &color) const;
};

inline namespace holder {
shader::font_t font{};
shader::basic_t basic{};
shader::shape_t shape{};
shader::circle_t circle{};
shader::striped_t striped{};

std::vector<base_t *> shaders{&font, &basic, &shape, &circle, &striped};

void init() {
  for (base_t *ref : shaders) {
    ref->init();
  }
}
} // namespace holder
} // namespace shader