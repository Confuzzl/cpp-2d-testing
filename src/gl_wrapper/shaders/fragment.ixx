module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:fragment;

import glm;

export namespace shaders {
namespace frag {
struct basic {
  static constexpr char name[] = "basic.frag";

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(frag_color); }
};
struct circle {
  static constexpr char name[] = "circle.frag";

  uniform<glm::vec2> center;
  uniform<float> radius;

  uniform<glm::uvec2> screen_dimensions;
  uniform<glm::mat4> view;

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(center);
    CREATE_UNIFORM(radius);
    CREATE_UNIFORM(screen_dimensions);
    CREATE_UNIFORM(view);
    CREATE_UNIFORM(frag_color);
  }
};
struct striped {
  static constexpr char name[] = "striped.frag";

  uniform<unsigned int> width;
  uniform<unsigned int> spacing;
  uniform<unsigned int> pattern;

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(width);
    CREATE_UNIFORM(spacing);
    CREATE_UNIFORM(pattern);
    CREATE_UNIFORM(frag_color);
  }
};
struct texcol {
  static constexpr char name[] = "texcol.frag";

  sampler_t sampler;
  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(sampler);
    CREATE_UNIFORM(frag_color);
  }
};
struct sdf_font {
  static constexpr char name[] = "sdf_font.frag";

  sampler_t sampler;
  uniform<glm::uvec3> frag_color;
  uniform<float> threshold;
  uniform<float> font_size;
  uniform<bool> anti_alias;

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(sampler);
    CREATE_UNIFORM(frag_color);
    CREATE_UNIFORM(threshold);
    CREATE_UNIFORM(font_size);
    CREATE_UNIFORM(anti_alias);
  }
};
} // namespace frag
} // namespace shaders