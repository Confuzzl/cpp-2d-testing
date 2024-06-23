module;

#include "util/gl.h"

module frame;

import <numbers>;
import app;
import shaders;
// import buffer_objects;
import vertex_layout;

// VBOHandle BaseFrame::POINT = VBO_HOLDER.get(sizeof(vertex::simple), 1);
// VBOHandle BaseFrame::LINE = VBO_HOLDER.get(sizeof(vertex::simple), 2);
// VBOHandle BaseFrame::TRI = VBO_HOLDER.get(sizeof(vertex::simple), 3);
// VBOHandle BaseFrame::QUAD = VBO_HOLDER.get(sizeof(vertex::simple), 4);

// BaseFrame::BaseFrame(const glm::mat4 &matrix) : matrix{matrix} {}

void BaseFrame::drawPoint(const glm::vec2 &point, const float size,
                          const color_t &color) const {
  static constexpr float SCALE = 0.002f;
  drawCircle(point, SCALE * size, color);
}
void BaseFrame::drawPointFixed(const glm::vec2 &point, const float size,
                               const color_t &color) const {
  POINT.write(point);

  shaders::basic.setView(matrix).setFragColor(color);
  glPointSize(size);
  shaders::basic.draw(GL_POINTS, POINT);
}

void BaseFrame::drawLine(const glm::vec2 &from, const glm::vec2 &to,
                         const float size, const color_t &color) const {
  LINE.write(from);
  LINE.write(to);

  shaders::line.setView(matrix).setFragColor(color).setThickness(size);
  shaders::line.draw(GL_LINES, LINE);
}
void BaseFrame::drawLineFixed(const glm::vec2 &from, const glm::vec2 &to,
                              const color_t &color) const {
  LINE.write(from);
  LINE.write(to);

  shaders::basic.setView(matrix).setFragColor(color);
  shaders::line.draw(GL_LINES, LINE);
}

void BaseFrame::drawArrow(const dimension_t &dimensions,
                          const color_t &color) const {
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

  for (const glm::vec2 vertex : vertices) {
    QUAD.write(vertex);
  }

  shaders::basic.setView(matrix).setFragColor(color);
  shaders::basic.draw(GL_LINES, QUAD, INDICES);
}

void BaseFrame::drawCircle(const glm::vec2 &center, const float radius,
                           const color_t &color) const {
  POINT.write(center);

  shaders::circ.setView(matrix)
      .setRadius(radius)
      .setCenter(center)
      .setScreenDimensions({App::WIDTH, App::HEIGHT})
      .setFragColor(color);
  shaders::circ.draw(GL_POINTS, POINT);
}

void BaseFrame::drawBox(const dimension_t &dimensions, const float lineSize,
                        const color_t &color) const {
  const auto &[from, to] = dimensions;

  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};
  drawLine(corners[0], corners[1], lineSize, color);
  drawLine(corners[1], corners[2], lineSize, color);
  drawLine(corners[2], corners[3], lineSize, color);
  drawLine(corners[3], corners[0], lineSize, color);
}

void BaseFrame::drawBoxFixed(const dimension_t &dimensions,
                             const float lineSize, const color_t &color) const {
  const auto &[from, to] = dimensions;

  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};

  for (const glm::vec2 corner : corners) {
    QUAD.write(corner);
  }

  shaders::basic.setView(matrix).setFragColor(color);
  shaders::basic.draw(GL_LINE_LOOP, QUAD);
}
void BaseFrame::drawQuad(const dimension_t &dimensions,
                         const color_t &color) const {
  const auto &[from, to] = dimensions;

  const glm::vec2 corners[4] = {{from.x, to.y}, from, to, {to.x, from.y}};

  for (const glm::vec2 corner : corners) {
    QUAD.write(corner);
  }

  shaders::basic.setView(matrix).setFragColor(color);
  shaders::basic.draw(GL_TRIANGLE_STRIP, QUAD);
}

import mesh;

void BaseFrame::drawMesh(const Mesh &mesh, const glm::vec2 &pos,
                         const float rot, const color_t &color,
                         const GLenum primitive) const {

  shaders::trans.setView(matrix)
      .setParentPos(pos)
      .setRotation(rot)
      .setFragColor(color);
  shaders::trans.draw(primitive, mesh.vbo);
}