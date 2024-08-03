module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:vertex;

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
      { t.enableVAO(vao) } -> std::same_as<void>;
    };

template <typename T> struct base : UniformHolder {
  using layout_t = T;

  using UniformHolder::UniformHolder;

  void enableVAO(GLuint &vao) { vertex_layout::enable<layout_t>(vao); }
};

struct identity : base<vertex_layout::pos> {
  static constexpr char name[] = "identity.vert";

  using base::base;

  // void createUniforms(const GLuint ID);
};

struct basic : base<vertex_layout::pos> {
  static constexpr char name[] = "basic.vert";

  NEW_UNIFORM(glm::mat4, view);

  using base::base;

  // NEW_UNIFORM(glm::mat4, view);

  // void createUniforms(const GLuint ID);
};

struct trans : base<vertex_layout::pos> {
  static constexpr char name[] = "trans.vert";

  NEW_UNIFORM(glm::vec2, parent_pos);
  NEW_UNIFORM(float, rotation);
  NEW_UNIFORM(glm::mat4, view);

  using base::base;

  // NEW_UNIFORM(glm::vec2, parent_pos);
  // NEW_UNIFORM(float, rotation);
  // NEW_UNIFORM(glm::mat4, view);

  // void createUniforms(const GLuint ID);
};
struct tex : base<vertex_layout::postex> {
  static constexpr char name[] = "tex.vert";

  NEW_UNIFORM(glm::mat4, view);

  using base::base;

  // NEW_UNIFORM(glm::mat4, view);

  // void createUniforms(const GLuint ID);
};
} // namespace vert
} // namespace shaders