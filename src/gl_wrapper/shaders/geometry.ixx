module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:geometry;

import glm;

export namespace shaders {
namespace geom {
struct line {
  static constexpr char name[] = "line.geom";

  uniform<glm::mat4> view;
  uniform<float> thickness;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(view);
    CREATE_UNIFORM(thickness);
  }
};
} // namespace geom
} // namespace shaders