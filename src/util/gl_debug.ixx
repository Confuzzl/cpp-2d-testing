module;

#include "util/gl.h"

export module gl_debug;

import debug;

export namespace GL {
std::string sourceName(GLenum source) {
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    return "API";
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    return "WINDOW SOURCE";
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    return "SHADER COMPILER";
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    return "THIRD PARTY";
  case GL_DEBUG_SOURCE_APPLICATION:
    return "APPLICATION";
  case GL_DEBUG_SOURCE_OTHER:
    return "OTHER";
  }
  return "";
}

std::string typeName(GLenum type) {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    return "ERROR";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "DEPRECATED BEHAVIOR";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "UNDEFINED BEHAVIOR";
  case GL_DEBUG_TYPE_PORTABILITY:
    return "PORTABILITY";
  case GL_DEBUG_TYPE_PERFORMANCE:
    return "PERFORMANCE";
  case GL_DEBUG_TYPE_MARKER:
    return "MARKER";
  case GL_DEBUG_TYPE_PUSH_GROUP:
    return "PUSH GROUP";
  case GL_DEBUG_TYPE_POP_GROUP:
    return "POP GROUP";
  case GL_DEBUG_TYPE_OTHER:
    return "OTHER";
  }
  return "";
}

std::string severityName(GLenum severity) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    return "HIGH";
  case GL_DEBUG_SEVERITY_MEDIUM:
    return "MEDIUM";
  case GL_DEBUG_SEVERITY_LOW:
    return "LOW";
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    return "NOTIFICATION";
  }
  return "";
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;
  println("source={} type={} id={} severity={} | {}", sourceName(source),
          typeName(type), id, severityName(severity), message);
}
} // namespace GL