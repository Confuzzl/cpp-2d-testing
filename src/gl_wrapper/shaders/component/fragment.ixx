module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:fragment;

import glm;

export namespace shaders {
namespace frag {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("frag");

struct basic : UniformHolder {
  static constexpr char name[] = "basic.frag";

  NEW_UNIFORM(glm::uvec3, frag_color);

  using UniformHolder::UniformHolder;
};
struct circle : UniformHolder {
  static constexpr char name[] = "circle.frag";

  NEW_UNIFORM(glm::vec2, center);
  NEW_UNIFORM(float, radius);
  NEW_UNIFORM(glm::uvec2, screen_dimensions);
  NEW_UNIFORM(glm::mat4, view);
  NEW_UNIFORM(glm::uvec3, frag_color);

  using UniformHolder::UniformHolder;
};
struct striped : UniformHolder {
  static constexpr char name[] = "striped.frag";

  NEW_UNIFORM(unsigned int, width);
  NEW_UNIFORM(unsigned int, spacing);
  NEW_UNIFORM(unsigned int, pattern);
  NEW_UNIFORM(glm::uvec3, frag_color);

  using UniformHolder::UniformHolder;
};
struct texcol : UniformHolder {
  static constexpr char name[] = "texcol.frag";

  Sampler sampler{programID, "sampler"};
  NEW_UNIFORM(glm::uvec3, frag_color);

  using UniformHolder::UniformHolder;
};
struct sdf_font : UniformHolder {
  static constexpr char name[] = "sdf_font.frag";

  Sampler sampler{programID, "sampler"};
  NEW_UNIFORM(glm::uvec3, frag_color);
  NEW_UNIFORM(float, threshold);
  NEW_UNIFORM(float, font_size);
  NEW_UNIFORM(bool, anti_alias);

  using UniformHolder::UniformHolder;
};
} // namespace frag
} // namespace shaders