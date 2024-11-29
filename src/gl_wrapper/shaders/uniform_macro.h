#pragma once

#define NEW_UNIFORM(type, name)                                                \
  Uniform<type> name { programID, #name }
#define NEW_COLOR NEW_UNIFORM(Color, frag_color)
#define NEW_SAMPLER                                                            \
  Sampler sampler { programID, "sampler" }
#define NEW_UNIFORM_BLOCK(type)                                                \
  GL::UniformBlock<shaders::uniform::type> type{programID};