module;

#include "util/gl.h"
#include <numbers>
#include <numeric>

export module mesh;

import vector;
import <functional>;
import <map>;
import <memory>;

import buffer_objects;
import vertices;
import poly;

template <typename T> struct BufferHolder {
  std::map<unsigned char, T> objects{};

  T &get(const unsigned char n) {
    if (!objects.contains(n))
      objects.emplace(n, T{n});
    return objects.at(n);
  }
};

export BufferHolder<vbo<vertex::simple>> VBOHolder{};
BufferHolder<radial_ebo> EBOHolder{};

export struct Mesh {
  std::vector<vertex::simple> localVertexData;
  const vbo<vertex::simple> &vbo;
  const radial_ebo &ebo;

  Mesh(const std::vector<vertex::simple> &data)
      : localVertexData{data},
        vbo{VBOHolder.get(static_cast<unsigned char>(data.size()))},
        ebo{EBOHolder.get(static_cast<unsigned char>(data.size()))} {}
};
