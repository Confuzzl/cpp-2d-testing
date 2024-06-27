module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:vertex;

using namespace shaders;
using namespace shaders::vert;

void identity::createUniforms(const GLuint ID) {}

uniform<glm::mat4> basic::view;
void basic::createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }

uniform<glm::vec2> trans::parent_pos;
uniform<float> trans::rotation;
uniform<glm::mat4> trans::view;
void trans::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(parent_pos);
  CREATE_UNIFORM(rotation);
  CREATE_UNIFORM(view);
}

uniform<glm::mat4> tex::view{};
void tex::createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }