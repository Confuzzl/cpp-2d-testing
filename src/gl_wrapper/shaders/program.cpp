module;

#include "util/gl.h"

#define SET_UNIFORM(type, func_name, param_t, param_name, shader)              \
  type &type::set##func_name(const param_t param_name) {                       \
    setUniform(shader.param_name, param_name);                                 \
    return *this;                                                              \
  }
#define SET_UNIFORM_V(type, func_name, param_t, param_name)                    \
  SET_UNIFORM(type, func_name, param_t, param_name, vertex)
#define SET_UNIFORM_F(type, func_name, param_t, param_name)                    \
  SET_UNIFORM(type, func_name, param_t, param_name, fragment)
#define SET_UNIFORM_G(type, func_name, param_t, param_name)                    \
  SET_UNIFORM(type, func_name, param_t, param_name, geometry)

#define BIND_TEXTURE(type, sampler_name)                                       \
  type &type::bindTexture(const GL::Texture &texture) {                        \
    glBindTextureUnit(fragment.sampler_name.binding, texture.ID);              \
    return *this;                                                              \
  }

module shaders:program;

import debug;

using namespace GL;

ProgramObject::ProgramObject(const std::initializer_list<Shader> &shaders)
    : ID{glCreateProgram()} {
  for (auto &shader : shaders)
    glAttachShader(ID, shader.ID);
  glLinkProgram(ID);
  for (auto &shader : shaders)
    glDetachShader(ID, shader.ID);
}
ProgramObject ::~ProgramObject() {
  if (ID)
    glDeleteProgram(ID);
}
ProgramObject::ProgramObject(ProgramObject &&o) : ID{o.ID} { o.ID = 0; };
ProgramObject &ProgramObject::operator=(ProgramObject &&o) {
  ID = o.ID;
  o.ID = 0;
  return *this;
}

using namespace shaders;

SET_UNIFORM_V(TexCol, View, glm::mat4 &, view)
SET_UNIFORM_F(TexCol, FragColor, Color &, frag_color)
BIND_TEXTURE(TexCol, sampler)

SET_UNIFORM_V(Sdf, View, glm::mat4 &, view)
SET_UNIFORM_F(Sdf, FragColor, Color &, frag_color)
SET_UNIFORM_F(Sdf, Threshold, float, threshold)
SET_UNIFORM_F(Sdf, FontSize, float, font_size)
SET_UNIFORM_F(Sdf, AntiAlias, bool, anti_alias)
BIND_TEXTURE(Sdf, sampler)

SET_UNIFORM_V(Basic, View, glm::mat4 &, view)
SET_UNIFORM_F(Basic, FragColor, Color &, frag_color)

SET_UNIFORM_V(Transform, ParentPos, glm::vec2, parent_pos)
SET_UNIFORM_V(Transform, Rotation, float, rotation)
SET_UNIFORM_V(Transform, View, glm::mat4 &, view)
SET_UNIFORM_F(Transform, FragColor, Color &, frag_color)

SET_UNIFORM_V(Striped, View, glm::mat4 &, view)
SET_UNIFORM_F(Striped, Width, unsigned int, width)
SET_UNIFORM_F(Striped, Spacing, unsigned int, spacing)
Striped &Striped::setPattern(const Pattern pattern) {
  setUniform(fragment.pattern, static_cast<unsigned int>(pattern));
  return *this;
}
SET_UNIFORM_F(Striped, FragColor, Color &, frag_color)

SET_UNIFORM_V(Bezier, View, glm::mat4 &, view)
Bezier &Bezier::setPoints(const glm::vec2 p0, const glm::vec2 p1,
                          const glm::vec2 p2, const glm::vec2 p3) {
  setUniform(fragment.p0, p0);
  setUniform(fragment.p1, p1);
  setUniform(fragment.p2, p2);
  setUniform(fragment.p3, p3);
  return *this;
}
Bezier &Bezier::setColor(const Color color) {
  setColor(color, color);
  return *this;
}
Bezier &Bezier::setColor(const Color color0, const Color color1) {
  setUniform(fragment.color0, color0);
  setUniform(fragment.color1, color1);
  return *this;
}
SET_UNIFORM_F(Bezier, Thickness, float, thickness);
SET_UNIFORM_F(Bezier, StepCount, unsigned int, step_count);
SET_UNIFORM_F(Bezier, ScreenDimensions, glm::uvec2, screen_dimensions)
SET_UNIFORM_F(Bezier, Debug, bool, debug)
SET_UNIFORM_F(Bezier, World, bool, world)

// SET_UNIFORM_V(LineCapped, View, glm::mat4 &, view)
// LineCapped &LineCapped::setPoints(const glm::vec2 p0, const glm::vec2 p1) {
//   setUniform(fragment.p0, p0);
//   setUniform(fragment.p1, p1);
//   return *this;
// }
// SET_UNIFORM_F(LineCapped, Thickness, float, thickness)
// SET_UNIFORM_F(LineCapped, FragColor, Color &, frag_color)
//
// SET_UNIFORM_V(LineUncapped, View, glm::mat4 &, view)
// LineUncapped &LineUncapped::setPoints(const glm::vec2 p0, const glm::vec2 p1)
// {
//   setUniform(fragment.p0, p0);
//   setUniform(fragment.p1, p1);
//   return *this;
// }
// SET_UNIFORM_F(LineUncapped, Thickness, float, thickness)
// SET_UNIFORM_F(LineUncapped, FragColor, Color &, frag_color)

SET_UNIFORM_V(Debug, View, glm::mat4 &, view);

// SET_UNIFORM_V(Line, View, glm::mat4 &, view)
SET_UNIFORM_G(Line, View, glm::mat4 &, view)
SET_UNIFORM_G(Line, Thickness, float, thickness)
// SET_UNIFORM_G(Line, World, bool, world)
// SET_UNIFORM_G(Line, ScaleMatrix, glm::mat4 &, scale_matrix);
SET_UNIFORM_F(Line, FragColor, Color &, frag_color)

SET_UNIFORM_G(Circle, View, glm::mat4 &, view)
SET_UNIFORM_G(Circle, Radius, float, radius)
SET_UNIFORM_F(Circle, Center, glm::vec2, center)
SET_UNIFORM_F(Circle, ScreenDimensions, glm::uvec2, screen_dimensions)
SET_UNIFORM_F(Circle, FragColor, Color &, frag_color)