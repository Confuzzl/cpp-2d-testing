module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:program;

import debug;

using namespace shaders;

SET_UNIFORM_V(TexCol, View, glm::mat4 &, view)
SET_UNIFORM_F(TexCol, FragColor, color_t &, frag_color)
BIND_TEXTURE(TexCol, sampler);

SET_UNIFORM_V(Sdf, View, glm::mat4 &, view)
SET_UNIFORM_F(Sdf, FragColor, color_t &, frag_color)
SET_UNIFORM_F(Sdf, Threshold, float, threshold)
SET_UNIFORM_F(Sdf, FontSize, float, font_size)
SET_UNIFORM_F(Sdf, AntiAlias, bool, anti_alias)
BIND_TEXTURE(Sdf, sampler);

SET_UNIFORM(Basic, View, glm::mat4 &, view, vertex)
SET_UNIFORM(Basic, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(Transform, ParentPos, glm::vec2, parent_pos, vertex)
SET_UNIFORM(Transform, Rotation, float, rotation, vertex)
SET_UNIFORM(Transform, View, glm::mat4 &, view, vertex)
SET_UNIFORM(Transform, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(Striped, View, glm::mat4 &, view, vertex)
SET_UNIFORM(Striped, Width, unsigned int, width, fragment)
SET_UNIFORM(Striped, Spacing, unsigned int, spacing, fragment)
Striped &Striped::setPattern(const Pattern pattern) {
  setUniform(fragment.pattern, static_cast<unsigned int>(pattern));
  return *this;
}
SET_UNIFORM(Striped, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(Line, View, glm::mat4 &, view, geometry)
SET_UNIFORM(Line, Thickness, float, thickness, geometry)
SET_UNIFORM(Line, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(Circle, View, glm::mat4 &, view, geometry)
SET_UNIFORM(Circle, Radius, float, radius, geometry)
SET_UNIFORM(Circle, Center, glm::vec2, center, fragment)
SET_UNIFORM(Circle, ScreenDimensions, glm::uvec2, screen_dimensions, fragment)
SET_UNIFORM(Circle, FragColor, color_t &, frag_color, fragment)