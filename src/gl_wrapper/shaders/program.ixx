module;

#include "gl_wrapper/shaders/uniform.h"

#define SET_UNIFORM_TEMPLATE(type, call)                                       \
  template <>                                                                  \
  void setUniform<type>(const shaders::Uniform<type> &uniform,                 \
                        const type &value) const {                             \
    call;                                                                      \
  }
#define SET_SCALAR(type, scalar_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, glProgramUniform1##scalar_type(ID, uniform.location, value))
#define SET_VECTOR(type, vector_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, glProgramUniform##vector_type##v(ID, uniform.location, 1,          \
                                             glm::value_ptr(value)))
#define SET_MATRIX(type, matrix_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, glProgramUniformMatrix##matrix_type##fv(                           \
                ID, uniform.location, 1, GL_FALSE, glm::value_ptr(value)))

export module shaders:program;

import <vector>;
import texture;
import glm;
import color;
import bo_heap;

import :shader;
import :vertex;
import :fragment;
import :geometry;

import vao;

import debug;

namespace GL {
struct ProgramObject {
  GLuint ID;

  ProgramObject(const std::initializer_list<Shader> &shaders);
  ~ProgramObject();
  ProgramObject(const ProgramObject &) = delete;
  ProgramObject(ProgramObject &&o);
  ProgramObject &operator=(const ProgramObject &) = delete;
  ProgramObject &operator=(ProgramObject &&o);

  template <typename T>
  void setUniform(const shaders::Uniform<T> &uniform, const T &value) const;

  SET_SCALAR(bool, i)
  SET_SCALAR(unsigned int, ui)
  SET_SCALAR(float, f)
  SET_MATRIX(glm::mat4, 4)
  SET_VECTOR(glm::vec2, 2f)
  SET_VECTOR(glm::uvec2, 2ui)
  SET_VECTOR(glm::uvec3, 3ui)
};
} // namespace GL

export namespace shaders {
template <vert::format V, frag::format F>
struct BaseProgram : ::GL::ProgramObject {
protected:
  GL::VertexArrayObject<typename V::layout_t> vao;
  V vertex;
  F fragment;

  BaseProgram(const std::initializer_list<GL::Shader> &shaders)
      : ProgramObject(shaders), vertex{ID}, fragment{ID} {}

private:
  void bind(const VBOHandle &vbo) const {
    glUseProgram(ID);
    glBindVertexArray(vao.ID);
    glVertexArrayVertexBuffer(vao.ID, 0, vbo->parent->ID, vbo->offset,
                              static_cast<GLsizei>(vbo->vertexSize));
  }
  void bind(const VBOHandle &vbo, const EBOHandle &ebo) const {
    bind(vbo);
    glVertexArrayElementBuffer(vao.ID, ebo->parent->ID);
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
};

template <vert::format V, frag::format F>
struct SimpleProgram : BaseProgram<V, F> {
  SimpleProgram()
      : BaseProgram<V, F>(
            {{GL_VERTEX_SHADER, V::name}, {GL_FRAGMENT_SHADER, F::name}}) {}
};

struct TexCol : SimpleProgram<vert::tex, frag::texcol> {
  TexCol &setView(const glm::mat4 &view);
  TexCol &setFragColor(const Color &frag_color);
  TexCol &bindTexture(const GL::Texture &texture);
};

struct Sdf : SimpleProgram<vert::tex, frag::sdf_font> {
  Sdf &setView(const glm::mat4 &view);
  Sdf &setFragColor(const Color &frag_color);
  Sdf &setThreshold(const float threshold);
  Sdf &setFontSize(const float font_size);
  Sdf &setAntiAlias(const bool anti_alias);
  Sdf &bindTexture(const GL::Texture &texture);
};
struct Basic : SimpleProgram<vert::basic, frag::basic> {
  Basic &setView(const glm::mat4 &view);
  Basic &setFragColor(const Color &frag_color);
};
struct Transform : SimpleProgram<vert::trans, frag::basic> {
  Transform &setParentPos(const glm::vec2 parent_pos);
  Transform &setRotation(const float rotation);
  Transform &setView(const glm::mat4 &view);
  Transform &setFragColor(const Color &frag_color);
};
struct Striped : SimpleProgram<vert::basic, frag::striped> {
  Striped &setView(const glm::mat4 &view);
  Striped &setWidth(const unsigned int width);
  Striped &setSpacing(const unsigned int spacing);
  enum Pattern { FORWARD = 1, BACKWARD = 2, CROSS = 3 };
  Striped &setPattern(const Pattern pattern = FORWARD);
  Striped &setFragColor(const Color &frag_color);
};

template <vert::format V, frag::format F, geom::format G>
struct GeometryProgram : BaseProgram<V, F> {
  G geometry;

  GeometryProgram()
      : BaseProgram<V, F>({{GL_VERTEX_SHADER, V::name},
                           {GL_FRAGMENT_SHADER, F::name},
                           {GL_GEOMETRY_SHADER, G::name}}),
        geometry{GL::ProgramObject::ID} {}
};

struct Line : GeometryProgram<vert::identity, frag::basic, geom::line> {
  Line &setView(const glm::mat4 &view);
  Line &setThickness(const float thickness);
  Line &setFragColor(const Color &frag_color);
};

struct Circle : GeometryProgram<vert::identity, frag::circle, geom::circle> {
  Circle &setView(const glm::mat4 &view);
  Circle &setRadius(const float radius);
  Circle &setCenter(const glm::vec2 center);
  Circle &setScreenDimensions(const glm::uvec2 screen_dimensions);
  Circle &setFragColor(const Color &frag_color);
};
} // namespace shaders