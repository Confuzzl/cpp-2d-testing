module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:geometry;

using namespace shaders;
using namespace shaders::geom;

uniform<glm::mat4> line::view;
uniform<float> line::thickness;
void line::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(view);
  CREATE_UNIFORM(thickness);
}

uniform<glm::mat4> circle::view;
uniform<float> circle::radius;
void circle::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(view);
  CREATE_UNIFORM(radius);
}