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

  uniform<glm::mat4> ortho;
  uniform<bool> perspective;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(view);
    CREATE_UNIFORM(thickness);
    CREATE_UNIFORM(ortho);
    CREATE_UNIFORM(perspective);
  }
};
} // namespace geom
} // namespace shaders