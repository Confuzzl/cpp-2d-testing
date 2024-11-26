module;

#include "gl_wrapper/shaders/uniform_macro.h"
#include "util/gl.h"

export module shaders:geometry;

import glm;
import uniform;

export namespace shaders {
namespace geom {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("geom");

struct line : UniformHolder {
  static constexpr char name[] = "line.geom";

  NEW_UNIFORM(float, thickness);
};
struct circle : UniformHolder {
  static constexpr char name[] = "circle.geom";

  NEW_UNIFORM(float, radius);
};
} // namespace geom
} // namespace shaders