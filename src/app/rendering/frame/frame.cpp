module;

#include "util/gl.h"
#include "util/main_objects.h"

module frame;

import <numbers>;
import app;
// import vertex_layout;
import bo_heap;
import aabb;
import debug;
import fbo;
import ubo;

void BaseFrame::drawPoint(const glm::vec2 point, const float size,
                          const Color color) const {
  static constexpr float SCALE = 0.002f;
  drawCircle(point, SCALE * size, color);
}
void BaseFrame::drawPointFixed(const glm::vec2 point, const float size,
                               const Color color) const {
  VBO_1->write(point);

  glPointSize(size);
  SHADERS.basic.setFragColor(color).draw(GL_POINTS, VBO_1);
}
void BaseFrame::drawLinePerspective(const BoundingBox &dimensions,
                                    const float thickness,
                                    const Color color) const {
  const auto [from, to] = dimensions;

  VBO_2->write(from);
  VBO_2->write(to);

  SHADERS.line.setFragColor(color).setThickness(thickness).draw(GL_LINES,
                                                                VBO_2);
}
void BaseFrame::drawLineConstant(const BoundingBox &dimensions,
                                 const float thickness,
                                 const Color color) const {
  drawLinePerspective(dimensions, thickness / MAIN_CAMERA.zoom(), color);
}
void BaseFrame::drawLine(const BoundingBox &dimensions,
                         const Color color) const {
  const auto [from, to] = dimensions;

  VBO_2->write(from);
  VBO_2->write(to);

  SHADERS.basic.setFragColor(color).draw(GL_LINES, VBO_2);
}

void BaseFrame::drawArrow(const BoundingBox &dimensions,
                          const Color color) const {
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

  SHADERS.basic.setFragColor(color).draw(GL_LINES, VBO_4, INDICES);
}

void BaseFrame::drawCircle(const glm::vec2 center, const float radius,
                           const Color color) const {
  VBO_1->write(center);

  SHADERS.circ.setRadius(radius).setCenter(center).setFragColor(color).draw(
      GL_POINTS, VBO_1);
}

void BaseFrame::drawBoxPerspective(const BoundingBox &dimensions,
                                   const float thickness,
                                   const Color color) const {
  const auto [from, to] = dimensions;

  const auto corners = dimensions.toLineLoop();

  VBO_4->write(corners);

  SHADERS.line.setFragColor(color).setThickness(thickness).draw(GL_LINE_LOOP,
                                                                VBO_4);
}
void BaseFrame::drawBoxConstant(const BoundingBox &dimensions,
                                const float thickness,
                                const Color color) const {
  drawBoxPerspective(dimensions, thickness / MAIN_CAMERA.zoom(), color);
}
void BaseFrame::drawBox(const BoundingBox &dimensions,
                        const Color color) const {
  const auto [from, to] = dimensions;

  const auto corners = dimensions.toLineLoop();

  VBO_4->write(corners);

  SHADERS.basic.setFragColor(color).draw(GL_LINE_LOOP, VBO_4);
}

void BaseFrame::drawQuad(const BoundingBox &dimensions,
                         const Color color) const {
  const auto [from, to] = dimensions;

  const auto corners = dimensions.toTriStrip();

  VBO_4->write(corners);

  SHADERS.basic.setFragColor(color).draw(GL_TRIANGLE_STRIP, VBO_4);
}

void BaseFrame::drawTexture(const BoundingBox &dimensions,
                            const GL::Texture &texture) {
  static VBOHandle VBO = VBO_HOLDER.get<vertex_layout::postex>(4);
  const vertex_layout::postex data[4]{
      {dimensions.min.x, dimensions.min.y, 0, 0},
      {dimensions.max.x, dimensions.min.y, TEXEL_RANGE, 0},
      {dimensions.min.x, dimensions.max.y, 0, TEXEL_RANGE},
      {dimensions.max.x, dimensions.max.y, TEXEL_RANGE, TEXEL_RANGE}};

  VBO->write(data);

  SHADERS.texcol.setFragColor(WHITE).bindTexture(texture).draw(
      GL_TRIANGLE_STRIP, VBO);
}
void BaseFrame::drawBoxBlur(const BoundingBox &dimensions,
                            const GL::Texture &texture,
                            const unsigned int radius,
                            const unsigned int direction) {
  static VBOHandle VBO = VBO_HOLDER.get<vertex_layout::postex>(4);
  const vertex_layout::postex data[4]{
      {dimensions.min.x, dimensions.min.y, 0, 0},
      {dimensions.max.x, dimensions.min.y, TEXEL_RANGE, 0},
      {dimensions.min.x, dimensions.max.y, 0, TEXEL_RANGE},
      {dimensions.max.x, dimensions.max.y, TEXEL_RANGE, TEXEL_RANGE}};

  VBO->write(data);

  SHADERS.boxblur.setDirection(direction)
      .setRadius(radius)
      .bindTexture(texture)
      .draw(GL_TRIANGLE_STRIP, VBO);
}
void BaseFrame::drawBoxBlur(const BoundingBox &dimensions,
                            const GL::Texture &texture,
                            const unsigned int radius) {
  static GL::FrameBufferObject fbo{};
  GL::Texture pass{texture.size};

  shaders::getUBO<shaders::uniform::ViewBlock>().update(
      {glm::ortho(texture.size)});
  glViewport(0, 0, texture.size.x, texture.size.y);
  fbo.bind();
  fbo.attachTexture(pass);
  drawBoxBlur({texture.size}, texture, radius, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  shaders::getUBO<shaders::uniform::ViewBlock>().update({matrix});
  glViewport(0, 0, App::WIDTH, App::HEIGHT);
  drawBoxBlur(dimensions, pass, radius, 1);
}

static GL::Texture outlinePass() {
  static constexpr glm::vec4 CLEAR{0.0, 0.0, 0.0, 0.0};
  GL::Texture out{App::DIMENSIONS, GL_NEAREST, GL_CLAMP_TO_BORDER};
  glTextureParameterfv(out.ID, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(CLEAR));
  return out;
}
void BaseFrame::drawOutline(const BoundingBox &dimensions,
                            const GL::Texture &texture,
                            const unsigned int thickness, const Color color) {
  static GL::FrameBufferObject fboA, fboB;
  static GL::Texture passA = outlinePass(), passB = outlinePass();
  static VBOHandle VBO = VBO_HOLDER.get<vertex_layout::postex>(4);
  static constexpr vertex_layout::postex data[4]{
      {0, 0, 0, 0},
      {App::DIMENSIONS.x, 0, TEXEL_RANGE, 0},
      {0, App::DIMENSIONS.y, 0, TEXEL_RANGE},
      {App::DIMENSIONS.x, App::DIMENSIONS.y, TEXEL_RANGE, TEXEL_RANGE}};

  fboA.bind();
  fboA.attachTexture(passA);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  drawTexture(dimensions, texture);

  shaders::getUBO<shaders::uniform::ViewBlock>().update({App::UI_MATRIX});

  fboB.bind();
  fboB.attachTexture(passB);
  glClear(GL_COLOR_BUFFER_BIT);

  VBO->write(data);
  SHADERS.outline.bindTexture(passA)
      .setThickness(thickness)
      .setDirection(0)
      .setFragColor(color)
      .draw(GL_TRIANGLE_STRIP, VBO);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  VBO->write(data);
  SHADERS.outline.bindTexture(passB)
      .setThickness(thickness)
      .setDirection(1)
      .setFragColor(color)
      .draw(GL_TRIANGLE_STRIP, VBO);

  shaders::getUBO<shaders::uniform::ViewBlock>().update({matrix});

  drawTexture(dimensions, texture);
}

void BaseFrame::drawMesh(const Mesh &mesh, const glm::vec2 &pos,
                         const float rot) const {
  static constexpr auto MAX_VERTICES = 0xffff;
  static VBOHandle VBO = VBO_HOLDER.get<>(MAX_VERTICES);

  VBO->write(mesh.data);

  SHADERS.trans.setParentPos(pos).setRotation(rot).setFragColor(mesh.color);

  if (mesh.ebo->parent) {
    SHADERS.trans.draw(mesh.primitive, VBO, mesh.ebo);
  } else {
    SHADERS.trans.draw(mesh.primitive, VBO);
  }
}

import aabb;

void BaseFrame::drawBezier(const Bezier &curve, const Color c0, const Color c1,
                           const float thickness,
                           unsigned int (*stepFunction)(const float,
                                                        const glm::vec2),
                           const bool world, const bool debug) {
  BoundingBox box;
  if (debug) {
    box.expand(curve.a);
    box.expand(curve.b);
    box.expand(curve.c);
    box.expand(curve.d);
    box.expand(thickness * 2.0f);
  } else {
    box = curve.box();
    box.expand(thickness);
  }

  VBO_4->write(box.toTriStrip());
  SHADERS.bezier.setPoints(curve.a, curve.b, curve.c, curve.d)
      .setColor(c0, c1)
      .setThickness(thickness)
      .setStepCount(stepFunction(MAIN_CAMERA.zoom(), box.size()))
      .setDebug(debug)
      .setWorld(world)
      .draw(GL_TRIANGLE_STRIP, VBO_4);
}