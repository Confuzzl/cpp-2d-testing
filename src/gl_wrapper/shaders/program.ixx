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

  GLuint ID;
  GLuint vao;

  ~BaseProgram() { glDeleteProgram(ID); }

private:
  std::vector<Shader> compileList;

protected:
  V vertex;
  F fragment;

  BaseProgram(std::vector<Shader> &&shaders)
      : ID{glCreateProgram()}, compileList{std::move(shaders)} {
    createShaders();
    createVAO();
    createUniforms();
  }

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
  void draw(const GLenum primitive, VBOHandle &vbo) const {
    bind(vbo);
    glDrawArrays(primitive, 0, vbo->count);
    vbo->reset();
  }
  void draw(const GLenum primitive, VBOHandle &vbo,
            const EBOHandle &ebo) const {
    bind(vbo, ebo);
    glDrawElements(
        primitive, ebo->length, GL_UNSIGNED_INT,
        reinterpret_cast<void *>(static_cast<GLuint64>(ebo->offset)));
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

struct TexCol : SimpleProgram<vert::tex, frag::texcol> {
  TexCol &setView(const glm::mat4 &view);
  TexCol &setFragColor(const color_t &frag_color);
  TexCol &bindTexture(const tex::texture &texture);
};

struct Sdf : SimpleProgram<vert::tex, frag::sdf_font> {
  Sdf &setView(const glm::mat4 &view);
  Sdf &setFragColor(const color_t &frag_color);
  Sdf &setThreshold(const float threshold);
  Sdf &setFontSize(const float font_size);
  Sdf &setAntiAlias(const bool anti_alias);
  Sdf &bindTexture(const tex::texture &texture);
};
struct Basic : SimpleProgram<vert::basic, frag::basic> {
  Basic &setView(const glm::mat4 &view);
  Basic &setFragColor(const color_t &frag_color);
};
struct Transform : SimpleProgram<vert::trans, frag::basic> {
  Transform &setParentPos(const glm::vec2 parent_pos);
  Transform &setRotation(const float rotation);
  Transform &setView(const glm::mat4 &view);
  Transform &setFragColor(const color_t &frag_color);
};
struct Striped : SimpleProgram<vert::basic, frag::striped> {
  Striped &setView(const glm::mat4 &view);
  Striped &setWidth(const unsigned int width);
  Striped &setSpacing(const unsigned int spacing);
  enum struct Pattern { FORWARD = 1, BACKWARD = 2, CROSS = 3 };
  Striped &setPattern(const Pattern pattern);
  Striped &setFragColor(const color_t &frag_color);
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

struct Line : GeometryProgram<vert::identity, frag::basic, geom::line> {
  Line &setView(const glm::mat4 &view);
  Line &setThickness(const float thickness);
  Line &setFragColor(const color_t &frag_color);
};

struct Circle : GeometryProgram<vert::identity, frag::circle, geom::circle> {
  Circle &setView(const glm::mat4 &view);
  Circle &setRadius(const float radius);
  Circle &setCenter(const glm::vec2 center);
  Circle &setScreenDimensions(const glm::uvec2 screen_dimensions);
  Circle &setFragColor(const color_t &frag_color);
};
} // namespace shaders