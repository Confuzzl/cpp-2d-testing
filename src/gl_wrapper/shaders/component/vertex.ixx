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

template <typename T> struct Base : UniformHolder {
  using layout_t = T;

  using UniformHolder::UniformHolder;

  void enableVAO(GLuint &vao) { vertex_layout::enable<layout_t>(vao); }
};

struct identity : Base<vertex_layout::pos> {
  static constexpr char name[] = "identity.vert";

  using Base::Base;
};

struct basic : Base<vertex_layout::pos> {
  static constexpr char name[] = "basic.vert";

  NEW_UNIFORM(glm::mat4, view);

  using Base::Base;
};

struct trans : Base<vertex_layout::pos> {
  static constexpr char name[] = "trans.vert";

  NEW_UNIFORM(glm::vec2, parent_pos);
  NEW_UNIFORM(float, rotation);
  NEW_UNIFORM(glm::mat4, view);

  using Base::Base;
};
struct tex : Base<vertex_layout::postex> {
  static constexpr char name[] = "tex.vert";

  NEW_UNIFORM(glm::mat4, view);

  using Base::Base;
};
} // namespace vert
} // namespace shaders