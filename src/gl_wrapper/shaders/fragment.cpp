module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:fragment;

using namespace shaders::frag;

void basic::createUniforms(const GLuint ID) { CREATE_UNIFORM(frag_color); }

void circle::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(center);
  CREATE_UNIFORM(radius);
  CREATE_UNIFORM(screen_dimensions);
  CREATE_UNIFORM(view);
  CREATE_UNIFORM(frag_color);
}

void striped::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(width);
  CREATE_UNIFORM(spacing);
  CREATE_UNIFORM(pattern);
  CREATE_UNIFORM(frag_color);
}

void texcol::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(sampler);
  CREATE_UNIFORM(frag_color);
}

void sdf_font::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(sampler);
  CREATE_UNIFORM(frag_color);
  CREATE_UNIFORM(threshold);
  CREATE_UNIFORM(font_size);
  CREATE_UNIFORM(anti_alias);
}
