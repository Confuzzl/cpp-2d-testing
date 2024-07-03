module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:fragment;

import :shader;
import glm;

export namespace shaders {
namespace frag {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("frag");

struct basic {
  static constexpr char name[] = "basic.frag";

  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID);
};
struct circle {
  static constexpr char name[] = "circle.frag";

  uniform<glm::vec2> center;
  uniform<float> radius;
  uniform<glm::uvec2> screen_dimensions;
  uniform<glm::mat4> view;
  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID);
};
struct striped {
  static constexpr char name[] = "striped.frag";

  uniform<unsigned int> width;
  uniform<unsigned int> spacing;
  uniform<unsigned int> pattern;
  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID);
};
struct texcol {
  static constexpr char name[] = "texcol.frag";

  sampler_t sampler;
  uniform<glm::uvec3> frag_color;

  void createUniforms(const GLuint ID);
};
struct sdf_font {
  static constexpr char name[] = "sdf_font.frag";

  sampler_t sampler;
  uniform<glm::uvec3> frag_color;
  uniform<float> threshold;
  uniform<float> font_size;
  uniform<bool> anti_alias;

  void createUniforms(const GLuint ID);
};
} // namespace frag
} // namespace shaders