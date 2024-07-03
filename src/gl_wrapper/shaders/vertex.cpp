module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:vertex;

using namespace shaders::vert;

void identity::createUniforms(const GLuint ID) {}

void basic::createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }

void trans::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(parent_pos);
  CREATE_UNIFORM(rotation);
  CREATE_UNIFORM(view);
}

void tex::createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }