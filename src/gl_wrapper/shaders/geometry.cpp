module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:geometry;

using namespace shaders::geom;

void line::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(view);
  CREATE_UNIFORM(thickness);
}

void circle::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(view);
  CREATE_UNIFORM(radius);
}