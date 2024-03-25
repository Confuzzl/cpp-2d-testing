module;

#include "util/gl.h"

module world_frame;

import app;
import <memory>;
import object;
import shader;
import buffer_objects;
import vertices;
import mesh;

void world::frame::render() const {
  drawGrid();
  for (const std::unique_ptr<base_obj_t> &obj : MAIN_SCENE.objs) {
    obj->draw();
  }
}

void world::frame::drawMesh(const Mesh &mesh, const glm::vec2 &pos,
                            const float rot, const color_t &color,
                            const GLenum primitive) const {
  shader::shape.use(mesh.vbo, mesh.ebo);

  GLintptr offset = 0;
  for (const vertex::simple &vertex : mesh.localVertexData) {
    glNamedBufferSubData(mesh.vbo.ID, offset, sizeof(vertex),
                         glm::value_ptr(vertex.pos));
    offset += sizeof(vertex);
  }

  shader::shape.setParentPos(pos)
      .setRotation(rot)
      .setView(MAIN_SCENE.camera.getView())
      .setFragColor(color);

  glDrawElements(primitive, mesh.ebo.count, GL_UNSIGNED_BYTE, 0);
}

void world::frame::drawGrid() const {
  static constexpr int HALF_SIZE = 10;
  static constexpr float WIDTH = 1;
  static constexpr unsigned int AXIS_COUNT = 2 * (HALF_SIZE * 2 + 1);
  static constexpr unsigned int VERTEX_COUNT = AXIS_COUNT * AXIS_COUNT;
  static vbo<vertex::simple> VBO{VERTEX_COUNT};
  static const glm::vec2 AXES[]{{1.0f, 0.0f}, {0.0f, 1.0f}};

  shader::basic.use(VBO);

  GLintptr offset = 0;
  for (int a = 0; a < 2; a++) {
    const glm::vec2 &axis = AXES[a];
    const glm::vec2 &other = AXES[1 - a];
    for (int i = -HALF_SIZE; i <= +HALF_SIZE; i++) {
      const glm::vec2 perpOffset = other * static_cast<float>(i);

      glNamedBufferSubData(
          VBO.ID, offset, sizeof(glm::vec2),
          glm::value_ptr(glm::vec2{axis * static_cast<float>(-HALF_SIZE)} +
                         perpOffset));
      offset += sizeof(glm::vec2);
      glNamedBufferSubData(
          VBO.ID, offset, sizeof(glm::vec2),
          glm::value_ptr(glm::vec2{axis * static_cast<float>(+HALF_SIZE)} +
                         perpOffset));
      offset += sizeof(glm::vec2);
    }
  }
  shader::basic.setView(MAIN_SCENE.camera.getView()).setFragColor(colors::GRAY);

  // glLineWidth(5);

  glDrawArrays(GL_LINES, 0, VERTEX_COUNT);
}

void world::frame::drawPoint(const glm::vec2 &point, const float size,
                             const color_t &color) const {
  static constexpr float SCALE = 0.002f;
  drawCircle(point, size * SCALE, color);
}
void world::frame::drawPointFixed(const glm::vec2 &point, const float size,
                                  const color_t &color) const {
  static vbo<vertex::simple> VBO{1};

  shader::basic.use(VBO);

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(point));

  shader::basic.setView(MAIN_SCENE.camera.getView()).setFragColor(color);

  glPointSize(size);

  glDrawArrays(GL_POINTS, 0, 1);
}

void world::frame::drawLine(const glm::vec2 &a, const glm::vec2 &b,
                            const float size, const color_t &color) const {
  static constexpr float SCALE = 0.0015f;
  static vbo<vertex::simple> VBO{4};
  static simple_ebo EBO{{0, 1, 2, 0, 2, 3}};

  const glm::vec2 dir = b - a;
  const glm::vec2 perp = glm::normalize(cwPerp(dir)) * size * SCALE;
  // a----->b
  // |
  // p
  // |
  // v
  const glm::vec2 vertices[4] = {a + perp, b + perp, b - perp, a - perp};

  shader::basic.use(VBO, EBO);

  GLintptr offset = 0;
  for (const glm::vec2 &vertex : vertices) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(vertex),
                         glm::value_ptr(vertex));
    offset += sizeof(vertex);
  }

  shader::basic.setView(MAIN_SCENE.camera.getView()).setFragColor(color);

  glDrawElements(GL_TRIANGLES, EBO.count, GL_UNSIGNED_BYTE, 0);
}
void world::frame::drawLineFixed(const glm::vec2 &a, const glm::vec2 &b,
                                 const float size, const color_t &color) const {
  static vbo<vertex::simple> VBO{2};

  shader::basic.use(VBO);

  glNamedBufferSubData(VBO.ID, 0, sizeof(glm::vec2), glm::value_ptr(a));
  glNamedBufferSubData(VBO.ID, sizeof(glm::vec2), sizeof(glm::vec2),
                       glm::value_ptr(b));

  shader::basic.setView(MAIN_SCENE.camera.getView()).setFragColor(color);

  // glLineWidth(size);

  glDrawArrays(GL_LINES, 0, 2);
}

void world::frame::drawArrow(const glm::vec2 &origin, const glm::vec2 &vector,
                             const color_t &color) const {
  drawArrowFromTo(origin, origin + vector, color);
}
import <numbers>;
void world::frame::drawArrowFromTo(const glm::vec2 &from, const glm::vec2 &to,
                                   const color_t &color) const {
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
  const glm::vec2 vertices[4] = {
      from, to,
      to + glm::vec2{head.x * COS - head.y * SIN, head.x * SIN + head.y * COS},
      to + glm::vec2{head.x * COS + head.y * SIN, head.y * COS - head.x * SIN}};

  shader::basic.use(VBO, EBO);

  GLintptr offset = 0;
  for (const glm::vec2 &vertex : vertices) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(vertex),
                         glm::value_ptr(vertex));
    offset += sizeof(vertex);
  }

  shader::basic.setView(MAIN_SCENE.camera.getView()).setFragColor(color);

  glDrawElements(GL_LINES, EBO.count, GL_UNSIGNED_BYTE, 0);
}

void world::frame::drawCircle(const glm::vec2 &center, const float radius,
                              const color_t &color) const {
  static vbo<vertex::simple> VBO{4};
  static simple_ebo EBO{{0, 1, 2, 0, 2, 3}};
  static const glm::vec2 CORNERS[4]{{-1, -1}, {+1, -1}, {+1, +1}, {-1, +1}};

  shader::circle.use(VBO, EBO);

  GLintptr offset = 0;
  for (const glm::vec2 &corner : CORNERS) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(corner),
                         glm::value_ptr(corner * radius + center));
    offset += sizeof(corner);
  }

  shader::circle.setCenter(center)
      .setRadius(radius)
      .setScreenDimensions({App::WIDTH, App::HEIGHT})
      .setView(MAIN_CAMERA.getView())
      .setFragColor(color);

  glDrawElements(GL_TRIANGLES, EBO.count, GL_UNSIGNED_BYTE, 0);
}

void world::frame::drawBox(const glm::vec2 &start, const glm::vec2 &size,
                           const float lineSize, const color_t &color) const {
  drawBoxFromTo(start, start + size, lineSize, color);
}
void world::frame::drawBoxFromTo(const glm::vec2 &from, const glm::vec2 &to,
                                 const float lineSize,
                                 const color_t &color) const {
  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};
  drawLine(corners[0], corners[1], lineSize, color);
  drawLine(corners[1], corners[2], lineSize, color);
  drawLine(corners[2], corners[3], lineSize, color);
  drawLine(corners[3], corners[0], lineSize, color);
}
void world::frame::drawBoxFixed(const glm::vec2 &start, const glm::vec2 &size,
                                const float lineSize,
                                const color_t &color) const {
  drawBoxFromToFixed(start, start + size, lineSize, color);
}
void world::frame::drawBoxFromToFixed(const glm::vec2 &from,
                                      const glm::vec2 &to, const float lineSize,
                                      const color_t &color) const {
  static vbo<vertex::simple> VBO{4};
  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};

  shader::basic.use(VBO);

  GLintptr offset = 0;
  for (const glm::vec2 &corner : corners) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(corner),
                         glm::value_ptr(corner));
    offset += sizeof(corner);
  }

  shader::basic.setView(MAIN_SCENE.camera.getView()).setFragColor(color);

  // glLineWidth(lineSize);

  glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void world::frame::drawQuad(const glm::vec2 &start, const glm::vec2 &size,
                            const color_t &color) const {
  drawQuadFromTo(start, start + size, color);
}
void world::frame::drawQuadFromTo(const glm::vec2 &from, const glm::vec2 &to,
                                  const color_t &color) const {
  static vbo<vertex::simple> VBO{4};
  static simple_ebo EBO{{0, 1, 2, 0, 2, 3}};
  const glm::vec2 corners[4] = {from, {to.x, from.y}, to, {from.x, to.y}};

  shader::basic.use(VBO, EBO);

  GLintptr offset = 0;
  for (const glm::vec2 &corner : corners) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(corner),
                         glm::value_ptr(corner));
    offset += sizeof(corner);
  }

  shader::basic.setView(MAIN_SCENE.camera.getView()).setFragColor(color);

  glDrawElements(GL_TRIANGLES, EBO.count, GL_UNSIGNED_BYTE, 0);
}