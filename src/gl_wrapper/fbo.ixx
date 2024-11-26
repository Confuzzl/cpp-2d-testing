module;

#include "util/gl.h"

export module fbo;

import texture;
import glm;

export namespace GL {
struct FrameBufferObject {
  GLuint ID;

  FrameBufferObject() { glGenFramebuffers(1, &ID); }
  ~FrameBufferObject() {
    if (ID)
      glDeleteFramebuffers(1, &ID);
  }

  void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, ID); }
  void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
  void attachTexture(const GL::Texture &texture) const {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture.ID, 0);
  }
};
} // namespace GL