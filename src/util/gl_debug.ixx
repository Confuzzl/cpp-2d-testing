module;

#include "util/gl.h"
#include <stdexcept>

export module gl_debug;

import debug;

std::string sourceName(const GLenum source) {
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
  throw std::runtime_error{"INVALID SOURCE ENUM"};
}

std::string typeName(const GLenum type) {
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
  throw std::runtime_error{"INVALID TYPE ENUM"};
}

std::string severityName(const GLenum severity) {
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
  throw std::runtime_error{"INVALID SEVERITY ENUM"};
}

export void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length,
                                   const GLchar *message,
                                   const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;
  println("source={} type={} id={} severity={} | {}", sourceName(source),
          typeName(type), id, severityName(severity), message);
}