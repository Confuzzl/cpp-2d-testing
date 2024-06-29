module;

#include "util/gl.h"

export module frame;

import glm;
import color;
import dimensions;

import buffer_object;

import vertex_layout;
import mesh;
import shaders;

import <format>;

export struct BaseFrame {
  glm::mat4 matrix{1.0f};

  virtual void render() = 0;

  void drawPoint(const glm::vec2 &point, const float size = 10,
                 const color_t &color = colors::WHITE) const;
  void drawPointFixed(const glm::vec2 &point, const float size = 10,
                      const color_t &color = colors::WHITE) const;

  void drawLine(const glm::vec2 &from, const glm::vec2 &to,
                const float size = 0.1,
                const color_t &color = colors::WHITE) const;
  void drawLineFixed(const glm::vec2 &from, const glm::vec2 &to,
                     const color_t &color = colors::WHITE) const;

  void drawArrow(const dimension_t &dimensions,
                 const color_t &color = colors::WHITE) const;

  void drawCircle(const glm::vec2 &center, const float radius,
                  const color_t &color = colors::WHITE) const;

  void drawBox(const dimension_t &dimensions, const float lineSize = 5,
               const color_t &color = colors::WHITE) const;
  void drawBoxFixed(const dimension_t &dimensions, const float lineSize = 5,
                    const color_t &color = colors::WHITE) const;
  void drawQuad(const dimension_t &dimensions,
                const color_t &color = colors::WHITE) const;

  template <shaders::vert::format V, shaders::frag::format F,
            is_vertex_layout T>
  void drawMesh(const shaders::simple_program_t<V, F> &shader,
                const Mesh<T> &mesh, const glm::vec2 &pos,
                const float rot) const {
    static_assert(std::same_as<V::layout_t, T>,
                  "shader vertex layout does not match mesh vertex layout");

    static constexpr auto MAX_VERTICES = 0xffff;
    static VBOHandle VBO = VBOHolder::getHandle<T>(MAX_VERTICES);

    // VBO.writeList(mesh.data);

    // shader.draw(mesh.primitive, VBO);
  }
};