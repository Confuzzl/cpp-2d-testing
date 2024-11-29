module;

#include "gl_wrapper/shaders/uniform_macro.h"
#include "util/gl.h"

export module shaders:geometry;

import glm;
import uniform;
import ubo;

export namespace shaders {
namespace geom {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("geom");

struct line : UniformHolder {
  static constexpr char name[] = "line.geom";

<<<<<<< HEAD
  NEW_UNIFORM_BLOCK(ViewBlock)

=======
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678
  NEW_UNIFORM(float, thickness);
};
struct circle : UniformHolder {
  static constexpr char name[] = "circle.geom";

<<<<<<< HEAD
  NEW_UNIFORM_BLOCK(ViewBlock)

=======
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678
  NEW_UNIFORM(float, radius);
};
} // namespace geom
} // namespace shaders