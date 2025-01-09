module;

#include "util/gl.h"

export module ubo;

import glm;
import buffer_object;

import <stdexcept>;
import <format>;
import debug;

export namespace GL {
template <typename T> struct UniformBufferObject : BufferObject {
  T data;

  UniformBufferObject() : BufferObject(sizeof(T), GL_DYNAMIC_STORAGE_BIT) {}

  template <typename U = T> void update(U &&data) {
    this->data = data;
    glNamedBufferSubData(ID, 0, sizeof(T), &this->data);
  }
};
} // namespace GL

export namespace shaders {
template <typename T> GL::UniformBufferObject<T> &getUBO() {
  static GL::UniformBufferObject<T> out;
  return out;
}

namespace uniform {
struct ViewBlock {
  static constexpr char name[] = "ViewBlock";
  glm::mat4 view;
};
struct ScreenBlock {
  static constexpr char name[] = "ScreenBlock";
  glm::uvec2 screen_dimensions;
};
} // namespace uniform
} // namespace shaders

export namespace GL {
template <typename T> struct UniformBlock {
  GLuint blockIndex;

  UniformBlock(const GLuint programID)
      : blockIndex{glGetUniformBlockIndex(programID, T::name)} {
    if (blockIndex == GL_INVALID_INDEX)
      throw std::runtime_error{
          std::format("INVALID BLOCK INDEX FOR {}", T::name)};
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, shaders::getUBO<T>().ID);
  }
};
} // namespace GL
