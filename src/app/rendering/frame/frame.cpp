module;

#include "util/gl.h"
#include "util/main_objects.h"

module frame;

import <numbers>;
import app;
import vertex_layout;
import bo_heap;

import debug;

void BaseFrame::drawPoint(const glm::vec2 point, const float size,
                          const Color &color) const {
  static constexpr float SCALE = 0.002f;
  drawCircle(point, SCALE * size, color);
}
void BaseFrame::drawPointFixed(const glm::vec2 point, const float size,
                               const Color &color) const {
  VBO_1->write(point);

  glPointSize(size);
  SHADERS.basic.setView(matrix).setFragColor(color).draw(GL_POINTS, VBO_1);
}

void BaseFrame::drawLine(const Dimension &dimensions, const float size,
                         const Color &color) const {
  const auto [from, to] = dimensions;

  VBO_2->write(from);
  VBO_2->write(to);

  SHADERS.line.setView(matrix).setFragColor(color).setThickness(size).draw(
      GL_LINES, VBO_2);
}
void BaseFrame::drawLineFixed(const Dimension &dimensions,
                              const Color &color) const {
  const auto [from, to] = dimensions;

  VBO_2->write(from);
  VBO_2->write(to);

  SHADERS.basic.setView(matrix).setFragColor(color).draw(GL_LINES, VBO_2);
}

void BaseFrame::drawArrow(const Dimension &dimensions,
                          const Color &color) const {
  const auto &[from, to] = dimensions;
  //   1
  //  /|\
  // 2 | 3
  //   |
  //   0
  static EBOHandle INDICES = EBO_HOLDER.get({0, 1, 1, 2, 1, 3});

  static constexpr float HEAD_SIZE = 0.5f;
  static constexpr float ANGLE_OFFSET = std::numbers::pi_v<float> / 4;
  static const float COS = std::cosf(ANGLE_OFFSET),
                     SIN = std::sinf(ANGLE_OFFSET);

  const glm::vec2 head = glm::normalize(from - to) * HEAD_SIZE;

  // a > 0
  // (x * cos - y * sin, x * sin + y * cos)
  // a < 0
  // (x * cos + y * sin, y * cos - x * sin)
  const glm::vec2 vertices[4]{
      from, to,
      to + glm::vec2{head.x * COS - head.y * SIN, head.x * SIN + head.y * COS},
      to + glm::vec2{head.x * COS + head.y * SIN, head.y * COS - head.x * SIN}};

  VBO_4->write(vertices);

  SHADERS.basic.setView(matrix).setFragColor(color).draw(GL_LINES, VBO_4,
                                                         INDICES);
}

void BaseFrame::drawCircle(const glm::vec2 center, const float radius,
                           const Color &color) const {
  VBO_1->write(center);

  SHADERS.circ.setView(matrix)
      .setRadius(radius)
      .setCenter(center)
      .setScreenDimensions({App::WIDTH, App::HEIGHT})
      .setFragColor(color)
      .draw(GL_POINTS, VBO_1);
}

void BaseFrame::drawBox(const Dimension &dimensions, const float lineSize,
                        const Color &color) const {
  const auto [from, to] = dimensions;

  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};
  drawLine({corners[0], corners[1]}, lineSize, color);
  drawLine({corners[1], corners[2]}, lineSize, color);
  drawLine({corners[2], corners[3]}, lineSize, color);
  drawLine({corners[3], corners[0]}, lineSize, color);
}

void BaseFrame::drawBoxFixed(const Dimension &dimensions,
                             const Color &color) const {
  const auto [from, to] = dimensions;

  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};

  VBO_4->write(corners);

  SHADERS.basic.setView(matrix).setFragColor(color).draw(GL_LINE_LOOP, VBO_4);
}
void BaseFrame::drawQuad(const Dimension &dimensions,
                         const Color &color) const {
  const auto [from, to] = dimensions;

  const glm::vec2 corners[4] = {
      from,
      {to.x, from.y},
      {from.x, to.y},
      to,
  };

  VBO_4->write(corners);

  SHADERS.basic.setView(matrix).setFragColor(color).draw(GL_TRIANGLE_STRIP,
                                                         VBO_4);
}

void BaseFrame::drawMesh(const Mesh &mesh, const glm::vec2 &pos,
                         const float rot) const {
  static constexpr auto MAX_VERTICES = 0xffff;
  static VBOHandle VBO = VBO_HOLDER.get<>(MAX_VERTICES);

  VBO->write(mesh.data);

  SHADERS.trans.setView(matrix).setParentPos(pos).setRotation(rot).setFragColor(
      mesh.color);

  if (mesh.ebo->parent) {
    SHADERS.trans.draw(mesh.primitive, VBO, mesh.ebo);
  } else {
    SHADERS.trans.draw(mesh.primitive, VBO);
  }
}

void BaseFrame::drawPolygon(const collision::Polygon &polygon,
                            const Color &color) const {
  VBOHandle vbo =
      VBO_HOLDER.get<>(static_cast<GLuint>(polygon.getVertices().size()));
  vbo->write(polygon.getVertices());
  SHADERS.basic.setView(matrix).setFragColor(color).draw(GL_TRIANGLE_FAN, vbo);
}
void BaseFrame::drawCircle(const collision::Circle &circle,
                           const Color &color) const {
  drawCircle(circle.getPos(), circle.getRadius(), color);
}