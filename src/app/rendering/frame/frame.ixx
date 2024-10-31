module;

#include "util/gl.h"

export module frame;

import glm;
import color;
import dimensions;
import collision;
import vertex_layout;
import mesh;

import <format>;

export struct BaseFrame {
  glm::mat4 matrix{1.0f};

  virtual void render() = 0;

  void drawPoint(const glm::vec2 point, const float size = 10,
                 const Color &color = colors::WHITE) const;
  void drawPointFixed(const glm::vec2 point, const float size = 10,
                      const Color &color = colors::WHITE) const;

  void drawLine(const Dimension &dimensions, const float size = 0.1,
                const Color &color = colors::WHITE) const;
  void drawLineFixed(const Dimension &dimensions,
                     const Color &color = colors::WHITE) const;

  void drawArrow(const Dimension &dimensions,
                 const Color &color = colors::WHITE) const;

  void drawCircle(const glm::vec2 center, const float radius,
                  const Color &color = colors::WHITE) const;

  void drawBox(const Dimension &dimensions, const float lineSize = 5,
               const Color &color = colors::WHITE) const;
  void drawBoxFixed(const Dimension &dimensions,
                    const Color &color = colors::WHITE) const;
  void drawQuad(const Dimension &dimensions,
                const Color &color = colors::WHITE) const;

  void drawMesh(const Mesh &mesh, const glm::vec2 &pos = {},
                const float rot = 0) const;

  void drawPolygon(const collision::Polygon &polygon,
                   const Color &color = colors::WHITE) const;
  void drawCircle(const collision::Circle &circle,
                  const Color &color = colors::WHITE) const;
};