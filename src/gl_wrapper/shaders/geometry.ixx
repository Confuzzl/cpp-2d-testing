module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:geometry;

import :shader;
import glm;

export namespace shaders {
namespace geom {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("geom");

struct line {
  static constexpr char name[] = "line.geom";

  uniform<glm::mat4> view;
  uniform<float> thickness;

  void createUniforms(const GLuint ID);
};
struct circle {
  static constexpr char name[] = "circle.geom";

  uniform<glm::mat4> view;
  uniform<float> radius;

  void createUniforms(const GLuint ID);
};
} // namespace geom
} // namespace shaders