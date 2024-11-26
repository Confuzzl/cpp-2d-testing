module;

#include "gl_wrapper/shaders/uniform_macro.h"
#include "util/gl.h"

export module shaders:vertex;

import glm;
import vertex_layout;
import uniform;
import ubo;

import debug;

export namespace shaders {
namespace vert {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("vert") &&
                   requires()
{
  typename T::layout_t;
};

template <typename T> struct Base : UniformHolder {
  using layout_t = T;

  using UniformHolder::UniformHolder;
};

struct identity : Base<vertex_layout::pos> {
  static constexpr char name[] = "identity.vert";

  using Base::Base;
};

struct basic : Base<vertex_layout::pos> {
  static constexpr char name[] = "basic.vert";

  using Base::Base;
};

struct trans : Base<vertex_layout::pos> {
  static constexpr char name[] = "trans.vert";

  NEW_UNIFORM(glm::vec2, parent_pos);
  NEW_UNIFORM(float, rotation);

  using Base::Base;
};
struct tex : Base<vertex_layout::postex> {
  static constexpr char name[] = "tex.vert";

  using Base::Base;
};
} // namespace vert
} // namespace shaders