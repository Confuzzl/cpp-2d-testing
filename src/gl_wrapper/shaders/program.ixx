module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:program;

import vector;
import texture;
import glm;
import color;
// import buffer_object;
import bo_heap;

import :shader;
import :vertex;
import :fragment;
import :geometry;

import debug;

export namespace shaders {
template <vert::format V, frag::format F> struct base_program_t {
  V vertex;
  F fragment;

  GLuint ID;
  GLuint vao;

  ~base_program_t() { glDeleteProgram(ID); }

private:
  std::vector<shader_t> compileList;

protected:
  base_program_t(std::vector<shader_t> &&shaders)
      : compileList{std::move(shaders)} {}

private:
  void createShaders() {
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
    glVertexArrayVertexBuffer(vao, 0, vbo.vboID, vbo.offset,
                              static_cast<GLsizei>(vbo.vertexSize));
  }
  void bind(const VBOHandle &vbo, const EBOHandle &ebo) const {
    bind(vbo);
    glVertexArrayElementBuffer(vao, ebo.eboID);
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
    glDrawArrays(primitive, 0, vbo.count);
    vbo.reset();
  }
  void draw(const GLenum primitive, VBOHandle &vbo,
            const EBOHandle &ebo) const {
    bind(vbo, ebo);
    glDrawElements(primitive, ebo.count, GL_UNSIGNED_INT,
                   reinterpret_cast<void *>(ebo.offset));
    vbo.reset();
  }

  template <typename T>
  void setUniform(const uniform<T> &uniform, const T &value) const;

  SET_SCALAR(bool, i)
  SET_SCALAR(unsigned int, ui)
  SET_SCALAR(float, f)
  template <>
  void setUniform<glm::mat4>(const uniform<glm::mat4> &uniform,
                             const glm::mat4 &value) const {
    glad_glProgramUniformMatrix4fv(ID, uniform.location, 1, 0,
                                   glm::value_ptr(value));
  }
  SET_VECTOR(glm::vec2, 2f)
  SET_VECTOR(glm::uvec2, 2ui)
  SET_VECTOR(glm::uvec3, 3ui)
};

template <vert::format V, frag::format F>
struct simple_program_t : base_program_t<V, F> {
  simple_program_t()
      : base_program_t<V, F>(
            vec::New<shader_t>(shader_t{GL_VERTEX_SHADER, V::name},
                               shader_t{GL_FRAGMENT_SHADER, F::name})) {}
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

template <vert::format V, frag::format F, geom::format G>
struct geometry_program_t : base_program_t<V, F> {
  G geometry;

  geometry_program_t()
      : base_program_t<V, F>(
            vec::New<shader_t>(shader_t{GL_VERTEX_SHADER, V::name},
                               shader_t{GL_FRAGMENT_SHADER, F::name},
                               shader_t{GL_GEOMETRY_SHADER, G::name})) {}

protected:
  void createUniforms() override {
    base_program_t<V, F>::createUniforms();
    geometry.createUniforms(this->ID);
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