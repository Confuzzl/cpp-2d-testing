module;

#include "util/gl.h"

export module object;

import glm;
import <memory>;

import mesh;
import poly;
import circle;
import polygon;
import app;
// import scene;
import color;
import func;
import rendering;
import world_frame;
import camera;
import shaders;

export struct base_obj_t {
  color_t color;

  base_obj_t(const color_t &color) : color{color} {};

  struct render_opts_t {
    GLenum primitive = GL_TRIANGLES;
  };
  virtual void draw(const render_opts_t &opts = {}) const = 0;
};

export template <typename colltype> struct Object;

export template <> struct Object<Polygon> : base_obj_t, Polygon {
  Mesh mesh;

  Object(Polygon &&poly, Mesh &&mesh, const color_t &color)
      : base_obj_t(color), Polygon(poly), mesh{std::move(mesh)} {}

  static std::unique_ptr<Object<Polygon>>
  from(Polygon &&poly, const color_t color = colors::random()) {
    Mesh mesh{func::map<glm::vec2, vertex::simple>(
        poly.getVertices(), [](const glm::vec2 &v) {
          return vertex::simple{v.x, v.y};
        })};
    return std::make_unique<Object<Polygon>>(std::move(poly), std::move(mesh),
                                             color);
  }

  void draw(const render_opts_t &opts = {}) const override {
    MAIN_RENDERER.worldFrame.drawMesh(mesh, pos(), rot(), color,
                                      opts.primitive);
  }
};
export template <> struct Object<Circle> : base_obj_t, Circle {
  Object(Circle &&circ, const color_t &color)
      : base_obj_t(color), Circle(circ) {}

  static std::unique_ptr<Object<Circle>>
  from(Circle &&circ, const color_t color = colors::random()) {
    return std::make_unique<Object<Circle>>(std::move(circ), color);
  }

  void draw(const render_opts_t &opts = {}) const override {
    MAIN_RENDERER.worldFrame.drawCircle(pos(), radius, color);
  }
};
