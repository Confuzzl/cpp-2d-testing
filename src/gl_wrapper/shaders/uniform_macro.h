#pragma once

#define NEW_UNIFORM(type, name)                                                \
  Uniform<type> name { programID, #name }
#define NEW_VIEW NEW_UNIFORM(glm::mat4, view)
#define NEW_COLOR NEW_UNIFORM(Color, frag_color)
#define NEW_SAMPLER                                                            \
  Sampler sampler { programID, "sampler" }