module;

#include "gl_wrapper/shaders/uniform_macro.h"
#include "util/gl.h"

export module shaders:fragment;

import glm;
import color;
import uniform;
import ubo;

export namespace shaders {
namespace frag {
template <typename T>
concept format = has_uniform<T> && has_extension<T>("frag");

struct basic : UniformHolder {
  static constexpr char name[] = "basic.frag";

  NEW_COLOR;

  using UniformHolder::UniformHolder;
};
struct circle : UniformHolder {
  static constexpr char name[] = "circle.frag";

  NEW_UNIFORM_BLOCK(ViewBlock)
  NEW_UNIFORM_BLOCK(ScreenBlock)

  NEW_UNIFORM(glm::vec2, center);
  NEW_UNIFORM(float, radius);
  NEW_COLOR;

  using UniformHolder::UniformHolder;
};
struct striped : UniformHolder {
  static constexpr char name[] = "striped.frag";

  NEW_UNIFORM(unsigned int, width);
  NEW_UNIFORM(unsigned int, spacing);
  NEW_UNIFORM(unsigned int, pattern);
  NEW_COLOR;

  using UniformHolder::UniformHolder;
};
struct texcol : UniformHolder {
  static constexpr char name[] = "texcol.frag";

  NEW_SAMPLER;
  NEW_COLOR;

  using UniformHolder::UniformHolder;
};
struct sdf_font : UniformHolder {
  static constexpr char name[] = "sdf_font.frag";

  NEW_SAMPLER;
  NEW_COLOR;
  NEW_UNIFORM(float, threshold);
  NEW_UNIFORM(float, font_size);
  NEW_UNIFORM(bool, anti_alias);

  using UniformHolder::UniformHolder;
};
struct bezier : UniformHolder {
  static constexpr char name[] = "bezier.frag";

  NEW_UNIFORM_BLOCK(ViewBlock)
  NEW_UNIFORM_BLOCK(ScreenBlock)

  NEW_UNIFORM(glm::vec2, p0);
  NEW_UNIFORM(glm::vec2, p1);
  NEW_UNIFORM(glm::vec2, p2);
  NEW_UNIFORM(glm::vec2, p3);
  NEW_UNIFORM(Color, color0);
  NEW_UNIFORM(Color, color1);
  NEW_UNIFORM(float, thickness);
  NEW_UNIFORM(unsigned int, step_count);
  NEW_UNIFORM(bool, world);
  NEW_UNIFORM(bool, debug);
};

struct debug : UniformHolder {
  static constexpr char name[] = "debug.frag";
};

struct box_blur : UniformHolder {
  static constexpr char name[] = "box_blur.frag";

  NEW_SAMPLER;
  NEW_UNIFORM(unsigned int, direction);
  NEW_UNIFORM(unsigned int, radius);
};
struct outline : UniformHolder {
  static constexpr char name[] = "outline.frag";

  NEW_SAMPLER;
  NEW_UNIFORM(unsigned int, thickness);
  NEW_UNIFORM(unsigned int, direction);
  NEW_COLOR;
};
} // namespace frag
} // namespace shaders