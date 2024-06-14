module;

#include "util/gl.h"

export module vertices;

import glm;

export namespace vertex {
struct simple {
  glm::vec2 pos;
  simple(const float x, const float y) : pos{x, y} {}

  const void *data() const { return &pos; }
};
struct font {
  glm::vec2 pos;
  glm::u16vec2 tex;
  font(const float x, const float y, const unsigned short u,
       const unsigned short v)
      : pos{x, y}, tex{u, v} {}

  const void *data() const { return &pos; }
};
} // namespace vertex