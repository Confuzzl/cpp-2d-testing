module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:geometry;

import glm;

export namespace shaders {
namespace geom {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("geom");

struct line : UniformHolder {
  static constexpr char name[] = "line.geom";

  NEW_UNIFORM(glm::mat4, view);
  NEW_UNIFORM(float, thickness);

  // void createUniforms(const GLuint ID);
};
struct circle : UniformHolder {
  static constexpr char name[] = "circle.geom";

  NEW_UNIFORM(glm::mat4, view);
  NEW_UNIFORM(float, radius);

  // void createUniforms(const GLuint ID);
};
} // namespace geom
} // namespace shaders