module;

#include "util/gl.h"

export module mesh;

import vector;

import poly;
import vertex_layout;
import buffer_object;
import color;

export template <is_vertex_layout vertex_t = vertex_layout::pos> struct Mesh {
  std::vector<vertex_t> data;
  int vboIndex;
  int eboIndex = -1;

  color_t color = colors::WHITE;
  GLenum primitive;
};