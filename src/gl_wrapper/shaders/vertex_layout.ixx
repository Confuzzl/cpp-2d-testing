module;

#include "util/gl.h"

export module vertex_layout;

import glm;
import gl_type_macro;

export namespace vertex_layout {
template <typename T>
void enable_helper(const GLuint ID, GLuint &index, GLint &offset) {
  glEnableVertexArrayAttrib(ID, index);
  glVertexArrayAttribFormat(ID, index, T::length(),
                            GL::macroOf<typename T::value_type>(), false,
                            offset);
  glVertexArrayAttribBinding(ID, index, 0);
  index++;
  offset += sizeof(T);
}
template <typename... inputs_t> void enable(const GLuint ID) {
  GLuint index = 0;
  GLint offset = 0;
  (enable_helper<inputs_t>(ID, index, offset), ...);
}

struct pos {
  glm::vec2 _pos;

  pos(const float x, const float y) : _pos{x, y} {}

  const void *data() const { return &_pos; }
};
template <> void enable<pos>(const GLuint ID) {
  enable<decltype(pos::_pos)>(ID);
}

struct postex {
  glm::vec2 pos;
  glm::u16vec2 tex;

  postex(const float x, const float y, const unsigned short u,
         const unsigned short v)
      : pos{x, y}, tex{u, v} {}

  const void *data() const { return &pos; }
};
template <> void enable<postex>(const GLuint ID) {
  enable<decltype(postex::pos), decltype(postex::tex)>(ID);
}
} // namespace vertex_layout