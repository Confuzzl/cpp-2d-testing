module;

#include "util/gl.h"

#define SET_UNIFORM_TEMPLATE(U, T, call)                                       \
  template <>                                                                  \
  void setUniform(const shaders::Uniform<U> &uniform, const T value) const {   \
    call;                                                                      \
  }
#define SET_SCALAR(type, scalar_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, type, glProgramUniform1##scalar_type(ID, uniform.location, value))
#define SET_VECTOR_2(type, vector_type)                                        \
  SET_UNIFORM_TEMPLATE(type, type,                                             \
                       glProgramUniform##vector_type##v(                       \
                           ID, uniform.location, 1, glm::value_ptr(value)))
#define SET_VECTOR(type, vector_type)                                          \
  SET_UNIFORM_TEMPLATE(type, type &,                                           \
                       glProgramUniform##vector_type##v(                       \
                           ID, uniform.location, 1, glm::value_ptr(value)))
#define SET_MATRIX(type, matrix_type)                                          \
  SET_UNIFORM_TEMPLATE(                                                        \
      type, type &,                                                            \
      glProgramUniformMatrix##matrix_type##fv(                                 \
          ID, uniform.location, 1, GL_FALSE, glm::value_ptr(value)))

#define SET_UNIFORM(func_name, param_t, param_name, shader, def)               \
  auto &set##func_name(const param_t param_name def) {                         \
    setUniform(shader.param_name, param_name);                                 \
    return *this;                                                              \
  }
#define SET_UNIFORM_V(func_name, param_t, param_name, def)                     \
  SET_UNIFORM(func_name, param_t, param_name, vertex, def)
#define SET_UNIFORM_F(func_name, param_t, param_name, def)                     \
  SET_UNIFORM(func_name, param_t, param_name, fragment, def)
#define SET_UNIFORM_G(func_name, param_t, param_name, def)                     \
  SET_UNIFORM(func_name, param_t, param_name, geometry, def)

#define BIND_TEXTURE(sampler_name)                                             \
  auto &bindTexture(const GL::Texture &texture) {                              \
    glBindTextureUnit(fragment.sampler_name.binding, texture.ID);              \
    return *this;                                                              \
  }

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

  template <typename U, typename T>
  void setUniform(const U &uniform, const T value) const;

  SET_SCALAR(bool, i)
  SET_SCALAR(int, i)
  SET_SCALAR(unsigned int, ui)
  SET_SCALAR(Color, ui)
  SET_SCALAR(float, f)
  SET_VECTOR_2(glm::vec2, 2f)
  SET_VECTOR_2(glm::uvec2, 2ui)
  SET_VECTOR(glm::uvec3, 3ui)
  SET_MATRIX(glm::mat4, 4)
};
} // namespace GL

export namespace shaders {
template <typename V, typename F> struct BaseProgram : ::GL::ProgramObject {
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

template <typename V, typename F> struct SimpleProgram : BaseProgram<V, F> {
  SimpleProgram()
      : BaseProgram<V, F>(
            {{GL_VERTEX_SHADER, V::name}, {GL_FRAGMENT_SHADER, F::name}}) {}
};

struct TexCol : SimpleProgram<vert::tex, frag::texcol> {
  SET_UNIFORM_F(FragColor, Color, frag_color, )
  BIND_TEXTURE(sampler);
};

struct Sdf : SimpleProgram<vert::tex, frag::sdf_font> {
  SET_UNIFORM_F(FragColor, Color, frag_color, )
  SET_UNIFORM_F(Threshold, float, threshold, )
  SET_UNIFORM_F(FontSize, float, font_size, )
  SET_UNIFORM_F(AntiAlias, bool, anti_alias, )
  BIND_TEXTURE(sampler);
};
struct Basic : SimpleProgram<vert::basic, frag::basic> {
  SET_UNIFORM_F(FragColor, Color, frag_color, )
};
struct Transform : SimpleProgram<vert::trans, frag::basic> {
  SET_UNIFORM_V(ParentPos, glm::vec2, parent_pos, = {})
  SET_UNIFORM_V(Rotation, float, rotation, = 0.0f)
  SET_UNIFORM_F(FragColor, Color, frag_color, )
};
struct Striped : SimpleProgram<vert::basic, frag::striped> {
  SET_UNIFORM_F(Width, unsigned int, width, = 1)
  SET_UNIFORM_F(Spacing, unsigned int, spacing, = 1)
  enum Pattern : unsigned int { FORWARD = 1, BACKWARD = 2, CROSS = 3 };
  SET_UNIFORM_F(Pattern, Pattern, pattern, = FORWARD)
  SET_UNIFORM_F(FragColor, Color, frag_color, )
};
struct Bezier : SimpleProgram<vert::basic, frag::bezier> {
  auto &setPoints(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2,
                  const glm::vec2 p3) {
    setUniform(fragment.p0, p0);
    setUniform(fragment.p1, p1);
    setUniform(fragment.p2, p2);
    setUniform(fragment.p3, p3);
    return *this;
  }
  auto &setColor(const Color color0, const Color color1) {
    setUniform(fragment.color0, color0);
    setUniform(fragment.color1, color1);
    return *this;
  }
  auto &setColor(const Color color = BLACK) { return setColor(color, color); }
  SET_UNIFORM_F(Thickness, float, thickness, )
  SET_UNIFORM_F(StepCount, unsigned int, step_count, )
  SET_UNIFORM_F(Debug, bool, debug, )
  SET_UNIFORM_F(World, bool, world, )
};
struct Debug : SimpleProgram<vert::basic, frag::debug> {};

struct BoxBlur : SimpleProgram<vert::tex, frag::box_blur> {
  BIND_TEXTURE(sampler);
  SET_UNIFORM_F(Direction, unsigned int, direction, )
  SET_UNIFORM_F(Radius, unsigned int, radius, )
};
struct Outline : SimpleProgram<vert::tex, frag::outline> {
  BIND_TEXTURE(sampler);
  SET_UNIFORM_F(Thickness, unsigned int, thickness, )
  SET_UNIFORM_F(Direction, unsigned int, direction, )
  SET_UNIFORM_F(FragColor, Color, frag_color, )
};

template <typename V, typename F, typename G>
struct GeometryProgram : BaseProgram<V, F> {
protected:
  G geometry;

  GeometryProgram()
      : BaseProgram<V, F>({{GL_VERTEX_SHADER, V::name},
                           {GL_FRAGMENT_SHADER, F::name},
                           {GL_GEOMETRY_SHADER, G::name}}),
        geometry{GL::ProgramObject::ID} {}
};

struct Line : GeometryProgram<vert::identity, frag::basic, geom::line> {
  SET_UNIFORM_G(Thickness, float, thickness, )
  SET_UNIFORM_F(FragColor, Color, frag_color, )
};

struct Circle : GeometryProgram<vert::identity, frag::circle, geom::circle> {
  SET_UNIFORM_G(Radius, float, radius, )
  SET_UNIFORM_F(Center, glm::vec2, center, )
  SET_UNIFORM_F(FragColor, Color, frag_color, )
};
} // namespace shaders