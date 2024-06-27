module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:fragment;

using namespace shaders;
using namespace shaders::frag;

uniform<glm::uvec3> basic::frag_color;
void basic::createUniforms(const GLuint ID) { CREATE_UNIFORM(frag_color); }

uniform<glm::vec2> circle::center;
uniform<float> circle::radius;
uniform<glm::uvec2> circle::screen_dimensions;
uniform<glm::mat4> circle::view;
uniform<glm::uvec3> circle::frag_color;
void circle::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(center);
  CREATE_UNIFORM(radius);
  CREATE_UNIFORM(screen_dimensions);
  CREATE_UNIFORM(view);
  CREATE_UNIFORM(frag_color);
}

uniform<unsigned int> striped::width;
uniform<unsigned int> striped::spacing;
uniform<unsigned int> striped::pattern;
uniform<glm::uvec3> striped::frag_color;
void striped::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(width);
  CREATE_UNIFORM(spacing);
  CREATE_UNIFORM(pattern);
  CREATE_UNIFORM(frag_color);
}

sampler_t texcol::sampler;
uniform<glm::uvec3> texcol::frag_color;
void texcol::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(sampler);
  CREATE_UNIFORM(frag_color);
}

sampler_t sdf_font::sampler;
uniform<glm::uvec3> sdf_font::frag_color;
uniform<float> sdf_font::threshold;
uniform<float> sdf_font::font_size;
uniform<bool> sdf_font::anti_alias;
void sdf_font::createUniforms(const GLuint ID) {
  CREATE_UNIFORM(sampler);
  CREATE_UNIFORM(frag_color);
  CREATE_UNIFORM(threshold);
  CREATE_UNIFORM(font_size);
  CREATE_UNIFORM(anti_alias);
}
