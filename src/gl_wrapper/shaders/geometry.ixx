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

  static uniform<glm::mat4> view;
  static uniform<float> thickness;

  static void createUniforms(const GLuint ID);
};
struct circle {
  static constexpr char name[] = "circle.geom";

  static uniform<glm::mat4> view;
  static uniform<float> radius;

  static void createUniforms(const GLuint ID);
};
} // namespace geom
} // namespace shaders