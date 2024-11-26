module;

#include "util/gl.h"

export module vertex_layout;

import glm;
import gl_type_macro;

export template <typename T>
concept is_vertex_layout = requires(const T t) {
  { t.data() } -> std::same_as<const void *>;
};

export namespace vertex_layout {
template <typename T>
void enable_helper(const GLuint ID, GLuint &index, GLint &offset) {
  glEnableVertexArrayAttrib(ID, index);
  glVertexArrayAttribFormat(ID, index, T::length(),
                            macroOf<typename T::value_type>(), false, offset);
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

  template <typename X, typename Y>
  constexpr pos(const X x, const Y y)
      : _pos{static_cast<float>(x), static_cast<float>(y)} {}
  constexpr pos(const glm::vec2 pos) : _pos{pos} {}

  const void *data() const { return &_pos; }
};
template <> void enable<pos>(const GLuint ID) { enable<glm::vec2>(ID); }

struct postex {
  glm::vec2 pos;
  glm::u16vec2 tex;

  template <typename X, typename Y, typename U, typename V>
  constexpr postex(const X x, const Y y, const U u, const V v)
      : pos{static_cast<float>(x), static_cast<float>(y)},
        tex{static_cast<unsigned short>(u), static_cast<unsigned short>(v)} {}
  constexpr postex(const glm::vec2 pos, const glm::u16vec2 tex)
      : pos{pos}, tex{tex} {}

  const void *data() const { return &pos; }
};
template <> void enable<postex>(const GLuint ID) {
  enable<glm::vec2, glm::u16vec2>(ID);
}
} // namespace vertex_layout