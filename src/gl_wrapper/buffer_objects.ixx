module;

#include "util/gl.h"

export module buffer_objects;

import vertices;
import vector;
import <numeric>;
import debug;

struct gl_buffer_obj {
  GLuint ID;
  gl_buffer_obj() { glCreateBuffers(1, &ID); }
};

export template <typename T> struct vbo : public gl_buffer_obj {
  vbo(const unsigned int n) : gl_buffer_obj() {
    glNamedBufferStorage(ID, n * sizeof(T), NULL, GL_DYNAMIC_STORAGE_BIT);
  }
};

export struct simple_ebo : public gl_buffer_obj {
  const unsigned int count;
  simple_ebo(const std::vector<GLubyte> &indices)
      : gl_buffer_obj(), count{static_cast<unsigned int>(indices.size())} {
    glNamedBufferData(ID, count * sizeof(GLubyte), indices.data(),
                      GL_STATIC_DRAW);
  }
};

export struct radial_ebo : public gl_buffer_obj {
  const unsigned int count;
  radial_ebo(const unsigned char n) : gl_buffer_obj(), count{n * 3u} {
    std::vector<GLubyte> indices{};
    indices.reserve(count);
    for (int i = 0; i < n - 2; i++) {
      indices.emplace_back(0);
      indices.emplace_back(i + 1);
      indices.emplace_back(i + 2);
    }
    indices.emplace_back(0);
    indices.emplace_back(n - 1);
    indices.emplace_back(1);
    glNamedBufferData(ID, count * sizeof(GLubyte), indices.data(),
                      GL_STATIC_DRAW);
  }
};