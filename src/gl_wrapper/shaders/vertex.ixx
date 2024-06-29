module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:vertex;

import :shader;
import glm;
import vertex_layout;
import <concepts>;

import debug;

export namespace shaders {
namespace vert {
template <typename T>
concept format =
    has_uniform<T> && has_extension<T>("vert") && requires(T t, GLuint &vao) {
      typename T::layout_t;
      { T::createVAO(vao) } -> std::same_as<void>;
    };

template <typename T> struct base {
  using layout_t = T;

  static void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    vertex_layout::enable<layout_t>(vao);
  }
};

struct identity : base<vertex_layout::pos> {
  static constexpr char name[] = "identity.vert";

  static void createUniforms(const GLuint ID);
};

struct basic : base<vertex_layout::pos> {
  static constexpr char name[] = "basic.vert";

  static uniform<glm::mat4> view;

  static void createUniforms(const GLuint ID);
};

struct trans : base<vertex_layout::pos> {
  static constexpr char name[] = "trans.vert";

  static uniform<glm::vec2> parent_pos;
  static uniform<float> rotation;
  static uniform<glm::mat4> view;

  static void createUniforms(const GLuint ID);
};
struct tex : base<vertex_layout::postex> {
  static constexpr char name[] = "tex.vert";

  static uniform<glm::mat4> view;

  static void createUniforms(const GLuint ID);
};
} // namespace vert
} // namespace shaders