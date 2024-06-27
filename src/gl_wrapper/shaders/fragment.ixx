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

  static uniform<glm::uvec3> frag_color;

  static void createUniforms(const GLuint ID);
};
struct circle {
  static constexpr char name[] = "circle.frag";

  static uniform<glm::vec2> center;
  static uniform<float> radius;
  static uniform<glm::uvec2> screen_dimensions;
  static uniform<glm::mat4> view;
  static uniform<glm::uvec3> frag_color;

  static void createUniforms(const GLuint ID);
};
struct striped {
  static constexpr char name[] = "striped.frag";

  static uniform<unsigned int> width;
  static uniform<unsigned int> spacing;
  static uniform<unsigned int> pattern;
  static uniform<glm::uvec3> frag_color;

  static void createUniforms(const GLuint ID);
};
struct texcol {
  static constexpr char name[] = "texcol.frag";

  static sampler_t sampler;
  static uniform<glm::uvec3> frag_color;

  static void createUniforms(const GLuint ID);
};
struct sdf_font {
  static constexpr char name[] = "sdf_font.frag";

  static sampler_t sampler;
  static uniform<glm::uvec3> frag_color;
  static uniform<float> threshold;
  static uniform<float> font_size;
  static uniform<bool> anti_alias;

  static void createUniforms(const GLuint ID);
};
} // namespace frag
} // namespace shaders