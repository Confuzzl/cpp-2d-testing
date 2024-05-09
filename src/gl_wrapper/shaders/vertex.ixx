module;

#include "gl_wrapper/shaders/uniform.h"

export module shaders:vertex;

import :shader;
import glm;
import <concepts>;

export namespace shaders {
namespace vert {
template <typename T>
concept format = has_uniform<T> && requires(T t, GLuint &vao) {
  { t.createVAO(vao) } -> std::same_as<void>;
};

struct basic {
  static constexpr char name[] = "basic.vert";

  uniform<glm::mat4> view;

  void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
  }

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }
};

struct trans {
  static constexpr char name[] = "trans.vert";

  uniform<glm::vec2> parent_pos;
  uniform<float> rotation;
  uniform<glm::mat4> view;

  void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
  }

  void createUniforms(const GLuint ID) {
    CREATE_UNIFORM(parent_pos);
    CREATE_UNIFORM(rotation);
    CREATE_UNIFORM(view);
  }
};
struct tex {
  static constexpr char name[] = "tex.vert";

  uniform<glm::mat4> view;

  void createVAO(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 2, GL_UNSIGNED_SHORT, false,
                              2 * sizeof(GLfloat));
    glVertexArrayAttribBinding(vao, 1, 0);
  }

  void createUniforms(const GLuint ID) { CREATE_UNIFORM(view); }
};
} // namespace vert
} // namespace shaders