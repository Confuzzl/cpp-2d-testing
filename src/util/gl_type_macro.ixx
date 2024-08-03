module;

#include "util/gl.h"

export module gl_type_macro;

export {
  template <typename T> constexpr GLenum macroOf();

  template <> constexpr GLenum macroOf<GLbyte>() { return GL_BYTE; }
  template <> constexpr GLenum macroOf<GLubyte>() { return GL_UNSIGNED_BYTE; }
  template <> constexpr GLenum macroOf<GLshort>() { return GL_SHORT; }
  template <> constexpr GLenum macroOf<GLushort>() { return GL_UNSIGNED_SHORT; }
  template <> constexpr GLenum macroOf<GLint>() { return GL_INT; }
  template <> constexpr GLenum macroOf<GLuint>() { return GL_UNSIGNED_INT; }
  template <> constexpr GLenum macroOf<GLfloat>() { return GL_FLOAT; }
  template <> constexpr GLenum macroOf<GLdouble>() { return GL_DOUBLE; }
} // namespace GL