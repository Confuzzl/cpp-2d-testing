module;

#include "gl_wrapper/shaders/uniform.h"

module shaders:program;

import debug;

using namespace shaders;

SET_UNIFORM_V(texcol_t, View, glm::mat4 &, view)
SET_UNIFORM_F(texcol_t, FragColor, color_t &, frag_color)
BIND_TEXTURE(texcol_t, sampler);

SET_UNIFORM_V(sdf_t, View, glm::mat4 &, view)
SET_UNIFORM_F(sdf_t, FragColor, color_t &, frag_color)
SET_UNIFORM_F(sdf_t, Threshold, float, threshold)
SET_UNIFORM_F(sdf_t, FontSize, float, font_size)
SET_UNIFORM_F(sdf_t, AntiAlias, bool, anti_alias)
BIND_TEXTURE(sdf_t, sampler);

SET_UNIFORM(basic_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(basic_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(trans_t, ParentPos, glm::vec2, parent_pos, vertex)
SET_UNIFORM(trans_t, Rotation, float, rotation, vertex)
SET_UNIFORM(trans_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(trans_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(striped_t, View, glm::mat4 &, view, vertex)
SET_UNIFORM(striped_t, Width, unsigned int, width, fragment)
SET_UNIFORM(striped_t, Spacing, unsigned int, spacing, fragment)
striped_t &striped_t::setPattern(const Pattern pattern) {
  setUniform(fragment::pattern, static_cast<unsigned int>(pattern));
  return *this;
}
SET_UNIFORM(striped_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(line_t, View, glm::mat4 &, view, geometry)
SET_UNIFORM(line_t, Thickness, float, thickness, geometry)
SET_UNIFORM(line_t, FragColor, color_t &, frag_color, fragment)

SET_UNIFORM(circ_t, View, glm::mat4 &, view, geometry)
SET_UNIFORM(circ_t, Radius, float, radius, geometry)
SET_UNIFORM(circ_t, Center, glm::vec2, center, fragment)
SET_UNIFORM(circ_t, ScreenDimensions, glm::uvec2, screen_dimensions, fragment)
SET_UNIFORM(circ_t, FragColor, color_t &, frag_color, fragment)