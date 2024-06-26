module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:vertex;

import :shader;
import glm;
import vertex_layout;
import <concepts>;

export namespace shaders {
namespace vert {
template <typename T>
concept format = has_uniform<T> && requires(T t, GLuint &vao) {
  typename T::layout_t;
  { t.createVAO(vao) } -> std::same_as<void>;
};

template <typename T> struct base {
  using layout_t = T;

  void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    vertex_layout::enable<layout_t>(vao);
  }
};

struct identity : base<vertex_layout::pos> {
  static constexpr char name[] = "identity.vert";

  void createUniforms(const GLuint ID) {}
};

struct basic : base<vertex_layout::pos> {
  static constexpr char name[] = "basic.vert";

  uniform<glm::mat4> view;

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }
};

struct trans : base<vertex_layout::pos> {
  static constexpr char name[] = "trans.vert";

  uniform<glm::vec2> parent_pos;
  uniform<float> rotation;
  uniform<glm::mat4> view;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(parent_pos);
    CREATE_UNIFORM(rotation);
    CREATE_UNIFORM(view);
  }
};
struct tex : base<vertex_layout::postex> {
  static constexpr char name[] = "tex.vert";

  uniform<glm::mat4> view;

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }
};
} // namespace vert
} // namespace shaders