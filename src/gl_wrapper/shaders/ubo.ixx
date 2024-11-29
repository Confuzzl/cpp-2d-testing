module;

#include "util/gl.h"

export module ubo;

import glm;
import buffer_object;

<<<<<<< HEAD
import <stdexcept>;
import <format>;
import debug;

export namespace GL {
template <typename T> struct UniformBufferObject : BufferObject {
  T data;

  UniformBufferObject() : BufferObject(sizeof(T)) {}
=======
export namespace GL {
template <typename T> struct UniformBufferObject : BufferObject {
  T data;
  GLuint blockIndex;

  UniformBufferObject(const char *name) : BufferObject(sizeof(T)) {
    blockIndex = glGetUniformBlockIndex(ID, name);
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, ID);
  }
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678

  void update(const T &data) {
    this->data = data;
    glNamedBufferSubData(ID, 0, sizeof(T), &this->data);
  }
};
} // namespace GL
<<<<<<< HEAD

export namespace shaders {
template <typename T> GL::UniformBufferObject<T> &getUBO() {
  static GL::UniformBufferObject<T> out;
=======
export namespace shaders {
template <typename T> GL::UniformBufferObject<T> &uniformBlock(const T &data) {
  static GL::UniformBufferObject<T> out{T::name};
  out.update(data);
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678
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
<<<<<<< HEAD
} // namespace shaders

export namespace GL {
template <typename T> struct UniformBlock {
  GLuint blockIndex;

  UniformBlock(const GLuint programID)
      : blockIndex{glGetUniformBlockIndex(programID, T::name)} {
    if (blockIndex == GL_INVALID_INDEX)
      throw std::runtime_error{
          std::format("INVALID BLOCK INDEX FOR {}", T::name)};
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, ::shaders::getUBO<T>().ID);
  }
};
} // namespace GL
=======
} // namespace shaders
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678
