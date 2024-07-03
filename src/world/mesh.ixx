module;

#include "util/gl.h"

export module mesh;

import vector;

import poly;
import vertex_layout;
import buffer_object;
import color;

export struct Mesh {
  std::vector<vertex_layout::pos> data;

  EBOHandle ebo{};

  color_t color;
  GLenum primitive;

  Mesh(std::vector<vertex_layout::pos> &&data, const GLenum primitive,
       const color_t &color = colors::WHITE)
      : data{std::move(data)}, primitive{primitive}, color{color} {}
};