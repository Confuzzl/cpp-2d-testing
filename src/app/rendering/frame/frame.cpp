module;

#include "util/gl.h"

module frame;

import <numbers>;
import app;
import shaders;
import buffer_objects;
import vertices;

BaseFrame::BaseFrame(const glm::mat4 &matrix) : matrix{matrix} {}

void BaseFrame::drawPoint(const glm::vec2 &point, const float size,
                          const color_t &color) const {
  static constexpr float SCALE = 0.002f;
  drawCircle(point, SCALE * size, color);
}
void BaseFrame::drawPointFixed(const glm::vec2 &point, const float size,
                               const color_t &color) const {
  static vbo<vertex::simple> VBO{1};

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(point));

  shaders::basic.use(VBO);
  shaders::basic.setView(matrix).setFragColor(color);

  glPointSize(size);
  glDrawArrays(GL_POINTS, 0, 1);
}

void BaseFrame::drawLine(const glm::vec2 &from, const glm::vec2 &to,
                         const float size, const color_t &color) const {
  static vbo<vertex::simple> VBO{2};

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(from));
  glNamedBufferSubData(VBO.ID, sizeof(glm::vec2), sizeof(glm::vec2),
                       glm::value_ptr(to));

  shaders::line.use(VBO);
  shaders::line.setView(matrix).setFragColor(color).setThickness(size);

  glDrawArrays(GL_LINES, 0, 2);
}
void BaseFrame::drawLineFixed(const glm::vec2 &from, const glm::vec2 &to,
                              const color_t &color) const {
  static vbo<vertex::simple> VBO{2};

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(from));
  glNamedBufferSubData(VBO.ID, sizeof(glm::vec2), sizeof(glm::vec2),
                       glm::value_ptr(to));

  shaders::basic.use(VBO);
  shaders::basic.setView(matrix).setFragColor(color);

  glDrawArrays(GL_LINES, 0, 2);
}

void BaseFrame::drawArrow(const dimension_t &dimensions,
                          const color_t &color) const {
  const auto &[from, to] = dimensions;
  //   1
  //  /|\
  // 2 | 3
  //   |
  //   0
  static vbo<vertex::simple> VBO{4};
  static simple_ebo EBO{{0, 1, 1, 2, 1, 3}};

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

  GLintptr offset = 0;
  for (const glm::vec2 &vertex : vertices) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(vertex),
                         glm::value_ptr(vertex));
    offset += sizeof(vertex);
  }

  shaders::basic.use(VBO, EBO);
  shaders::basic.setView(matrix).setFragColor(color);

  glDrawElements(GL_LINES, EBO.count, GL_UNSIGNED_BYTE, 0);
}

void BaseFrame::drawCircle(const glm::vec2 &center, const float radius,
                           const color_t &color) const {
  static vbo<vertex::simple> VBO{4};
  static const glm::vec2 CORNERS[4]{{-1, +1}, {-1, -1}, {+1, +1}, {+1, -1}};
  const glm::vec2 corners[4]{{center + glm::vec2{-radius, +radius}},
                             {center + glm::vec2{-radius, -radius}},
                             {center + glm::vec2{+radius, +radius}},
                             {center + glm::vec2{+radius, -radius}}};

  GLintptr offset = 0;
  for (const glm::vec2 &corner : corners) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(corner),
                         glm::value_ptr(corner));
    offset += sizeof(corner);
  }

  shaders::circle.use(VBO);
  shaders::circle.setCenter(center)
      .setRadius(radius)
      .setScreenDimensions({App::WIDTH, App::HEIGHT})
      .setView(matrix)
      .setFragColor(color);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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

  static vbo<vertex::simple> VBO{4};
  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};

  GLintptr offset = 0;
  for (const glm::vec2 &corner : corners) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(corner),
                         glm::value_ptr(corner));
    offset += sizeof(corner);
  }

  shaders::basic.use(VBO);
  shaders::basic.setView(matrix).setFragColor(color);

  glDrawArrays(GL_LINE_LOOP, 0, 4);
}
void BaseFrame::drawQuad(const dimension_t &dimensions,
                         const color_t &color) const {
  const auto &[from, to] = dimensions;

  static vbo<vertex::simple> VBO{4};
  const glm::vec2 corners[4] = {{from.x, to.y}, from, to, {to.x, from.y}};

  GLintptr offset = 0;
  for (const glm::vec2 &corner : corners) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(corner),
                         glm::value_ptr(corner));
    offset += sizeof(corner);
  }

  shaders::basic.use(VBO);
  shaders::basic.setView(matrix).setFragColor(color);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}