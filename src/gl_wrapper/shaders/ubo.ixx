module;

#include "util/gl.h"

export module ubo;

import glm;
import buffer_object;

export namespace GL {
template <typename T> struct UniformBufferObject : BufferObject {
  T data;
  GLuint blockIndex;

  UniformBufferObject(const char *name) : BufferObject(sizeof(T)) {
    blockIndex = glGetUniformBlockIndex(ID, name);
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, ID);
  }

  void update(const T &data) {
    this->data = data;
    glNamedBufferSubData(ID, 0, sizeof(T), &this->data);
  }
};
} // namespace GL
export namespace shaders {
template <typename T> GL::UniformBufferObject<T> &uniformBlock(const T &data) {
  static GL::UniformBufferObject<T> out{T::name};
  out.update(data);
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