module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:program;

import vector;
import texture;
import glm;
import color;
import bo_heap;

import :shader;
import :vertex;
import :fragment;
import :geometry;

import debug;

export namespace shaders {
template <vert::format V, frag::format F> struct BaseProgram {
  V vertex;
  F fragment;

  GLuint ID;
  GLuint vao;

  ~BaseProgram() { glDeleteProgram(ID); }

private:
  std::vector<Shader> compileList;

protected:
  BaseProgram(std::vector<Shader> &&shaders)
      : compileList{std::move(shaders)} {}

private:
  void createShaders() {
    println("PROGRAM: {}", ID);
    for (Shader &shader : compileList) {
      shader.compile();
      glAttachShader(ID, shader.ID);
    }
    glLinkProgram(ID);
    for (Shader &shader : compileList) {
      glDetachShader(ID, shader.ID);
      glDeleteShader(shader.ID);
    }
  }
  void createVAO() { vertex.createVAO(vao); }

protected:
  virtual void createUniforms() {
    vertex.createUniforms(ID);
    fragment.createUniforms(ID);
  }

private:
  void bind(const VBOHandle &vbo) const {
    glUseProgram(ID);
    glBindVertexArray(vao);
    glVertexArrayVertexBuffer(vao, 0, vbo->parent->ID, vbo->offset,
                              static_cast<GLsizei>(vbo->vertexSize));
  }
  void bind(const VBOHandle &vbo, const EBOHandle &ebo) const {
    bind(vbo);
    glVertexArrayElementBuffer(vao, ebo->parent->ID);
  }

public:
  void init() {
    ID = glCreateProgram();
    createShaders();
    createVAO();
    createUniforms();
  }

  void draw(const GLenum primitive, VBOHandle &vbo) const {
    bind(vbo);
    glDrawArrays(primitive, 0, vbo->count);
    vbo->reset();
  }
  void draw(const GLenum primitive, VBOHandle &vbo,
            const EBOHandle &ebo) const {
    bind(vbo, ebo);
    glDrawElements(primitive, ebo->length, GL_UNSIGNED_INT,
                   reinterpret_cast<void *>(ebo->offset));
    vbo->reset();
  }

  template <typename T>
  void setUniform(const uniform<T> &uniform, const T &value) const;

  SET_SCALAR(bool, i)
  SET_SCALAR(unsigned int, ui)
  SET_SCALAR(float, f)
  SET_MATRIX(glm::mat4, 4)
  SET_VECTOR(glm::vec2, 2f)
  SET_VECTOR(glm::uvec2, 2ui)
  SET_VECTOR(glm::uvec3, 3ui)
};

template <vert::format V, frag::format F>
struct SimpleProgram : BaseProgram<V, F> {
  SimpleProgram()
      : BaseProgram<V, F>(
            vec::New<Shader>(Shader{GL_VERTEX_SHADER, V::name},
                             Shader{GL_FRAGMENT_SHADER, F::name})) {}
};

struct texcol_t : SimpleProgram<vert::tex, frag::texcol> {
  texcol_t &setView(const glm::mat4 &view);
  texcol_t &setFragColor(const color_t &frag_color);
  texcol_t &bindTexture(const tex::texture &texture);
};

struct sdf_t : SimpleProgram<vert::tex, frag::sdf_font> {
  sdf_t &setView(const glm::mat4 &view);
  sdf_t &setFragColor(const color_t &frag_color);
  sdf_t &setThreshold(const float threshold);
  sdf_t &setFontSize(const float font_size);
  sdf_t &setAntiAlias(const bool anti_alias);
  sdf_t &bindTexture(const tex::texture &texture);
};
struct basic_t : SimpleProgram<vert::basic, frag::basic> {
  basic_t &setView(const glm::mat4 &view);
  basic_t &setFragColor(const color_t &frag_color);
};
struct trans_t : SimpleProgram<vert::trans, frag::basic> {
  trans_t &setParentPos(const glm::vec2 parent_pos);
  trans_t &setRotation(const float rotation);
  trans_t &setView(const glm::mat4 &view);
  trans_t &setFragColor(const color_t &frag_color);
};
struct striped_t : SimpleProgram<vert::basic, frag::striped> {
  striped_t &setView(const glm::mat4 &view);
  striped_t &setWidth(const unsigned int width);
  striped_t &setSpacing(const unsigned int spacing);
  enum struct Pattern { FORWARD = 1, BACKWARD = 2, CROSS = 3 };
  striped_t &setPattern(const Pattern pattern);
  striped_t &setFragColor(const color_t &frag_color);
};

template <vert::format V, frag::format F, geom::format G>
struct GeometryProgram : BaseProgram<V, F> {
  G geometry;

  GeometryProgram()
      : BaseProgram<V, F>(
            vec::New<Shader>(Shader{GL_VERTEX_SHADER, V::name},
                             Shader{GL_FRAGMENT_SHADER, F::name},
                             Shader{GL_GEOMETRY_SHADER, G::name})) {}

protected:
  void createUniforms() override {
    BaseProgram<V, F>::createUniforms();
    geometry.createUniforms(this->ID);
  }
};

struct line_t : GeometryProgram<vert::identity, frag::basic, geom::line> {
  line_t &setView(const glm::mat4 &view);
  line_t &setThickness(const float thickness);
  line_t &setFragColor(const color_t &frag_color);
};

struct circ_t : GeometryProgram<vert::identity, frag::circle, geom::circle> {
  circ_t &setView(const glm::mat4 &view);
  circ_t &setRadius(const float radius);
  circ_t &setCenter(const glm::vec2 center);
  circ_t &setScreenDimensions(const glm::uvec2 screen_dimensions);
  circ_t &setFragColor(const color_t &frag_color);
};
} // namespace shaders