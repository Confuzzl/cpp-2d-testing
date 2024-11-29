module;

#include "util/gl.h"
#include "util/main_objects.h"
#include <variant>

module world_frame;

import app;
import aabb;
import camera;
import mesh;
import glm;
import vertex_layout;
import bo_heap;
import shaders;
import math;
import ecs_component;
import <random>;
import debug;
import ubo;
import texture;
import fbo;

static glm::vec4 offsets() {
  return {random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f),
          random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f)};
}
static glm::vec2 point(const unsigned int i) {
  static glm::vec4 o[]{offsets(), offsets(), offsets(), offsets()};
  const double t = glfwGetTime();
  return {sin(o[i][0] * t + o[i][1]), sin(o[i][2] * t + o[i][3])};
}

void WorldFrame::render() {
  matrix = MAIN_CAMERA.getView();
  shaders::getUBO<shaders::uniform::ViewBlock>().update({matrix});

  drawGrid();

  // drawOutline({{-1, -1}, {1, 1}}, tex<"test2.png">(), 10, RED);

  // for (const auto [id, draw] : ECS.viewComponents<ecs::DirectRenderable>()) {
  //   (*draw)(this);
  //   draw->operator()(this);
  // }
  // for (const auto [id, rend] : ecs.viewComponents<ecs::Renderable>()) {
  //   drawMesh(rend->mesh);
  // }

  // for (const auto [id, box] : ECS.viewComponents<ecs::Boundable>()) {
  //   drawQuad({box->bounds.min, box->bounds.max},
  //            colors::random_i(id).setAlpha(127));
  // }

  //// drawBezier({point(0), point(1), point(2), point(3)}, colors::MAGENTA,
  ////            colors::CYAN, 0.01f, true);
  ////
  // drawBezier({{0, 0}, {0, 1}, {1, 1}, {2, 0}}, colors::BLUE, 0.01f, true);
  // drawBezier({{2, 0}, {3, -1}, {3, 0}, {3, 1}}, colors::RED, 0.01f, true);

  // static const auto spline = Spline::begin({{-3, 0}, {-3, 1}, {-2, 1}, {-1,
  // 0}})
  //                                .add({0, 0}, {0, 1})
  //                                .add({-1, 2}, {-2, 0})
  //     /*.endLoop()*/;
  // for (auto i = 0u; i < spline.pieces.size(); i++) {
  //   float t = static_cast<float>(i) / spline.pieces.size();
  //   const Color a = colors::mix(colors::MAGENTA, colors::YELLOW, t);
  //   const Color b =
  //       colors::mix(colors::MAGENTA, colors::YELLOW,
  //                   static_cast<float>(i + 1) / spline.pieces.size());
  //   drawBezier(spline.pieces[i], a, b);
  // }
}

void WorldFrame::drawGrid() const {
  static constexpr unsigned int GRID_RADIUS = 4;
  static constexpr float MAJOR_LINE_SPACING = 1.0f;
  static constexpr unsigned int MINOR_PER_MAJOR = 5;
  static constexpr float MINOR_LINE_SPACING =
      MAJOR_LINE_SPACING / MINOR_PER_MAJOR;
  static constexpr int NUM_HALF_MAJOR_LINES =
      static_cast<int>(GRID_RADIUS / MAJOR_LINE_SPACING);
  static constexpr int NUM_HALF_MINOR_LINES =
      NUM_HALF_MAJOR_LINES *
      (MINOR_PER_MAJOR - 1); // skip minor that overlaps major
  static VBOHandle MAJOR = VBO_HOLDER.get(NUM_HALF_MAJOR_LINES * 8);
  static VBOHandle MINOR = VBO_HOLDER.get(NUM_HALF_MINOR_LINES * 8);

  const float zoomLevel = MAIN_CAMERA.zoomExponent();

  const glm::vec2 pos = MAIN_CAMERA.getPos();
  const glm::vec2 ipos = {static_cast<int>(pos.x * zoomLevel) / zoomLevel,
                          static_cast<int>(pos.y * zoomLevel) / zoomLevel};

  const float majorSpacing = MAJOR_LINE_SPACING / zoomLevel;
  const float minorSpacing = MINOR_LINE_SPACING / zoomLevel;

  for (auto i = 0; i < NUM_HALF_MAJOR_LINES; i++) {
    const float min = -NUM_HALF_MAJOR_LINES * majorSpacing,
                max = +NUM_HALF_MAJOR_LINES * majorSpacing;
    const float majorGap = i * majorSpacing;

    for (auto j = 1; j < MINOR_PER_MAJOR; j++) {
      const float minorGap = j * minorSpacing;
      const float gap = majorGap + minorGap;
      const glm::vec2 minors[8]{{min, +gap}, {max, +gap}, {min, -gap},
                                {max, -gap}, {+gap, min}, {+gap, max},
                                {-gap, min}, {-gap, max}};
      for (const glm::vec2 p : minors)
        MINOR->write(ipos + p);
    }

    const glm::vec2 majors[8]{
        {min, +majorGap}, {max, +majorGap}, {min, -majorGap}, {max, -majorGap},
        {+majorGap, min}, {+majorGap, max}, {-majorGap, min}, {-majorGap, max}};
    for (const glm::vec2 p : majors)
      MAJOR->write(ipos + p);
  }
  SHADERS.line.setFragColor(LIGHTEST_GRAY)
      .setThickness(0.005f / MAIN_CAMERA.zoom())
      .draw(GL_LINES, MINOR);
  SHADERS.line.setFragColor(GRAY)
      .setThickness(0.005f / MAIN_CAMERA.zoom())
      .draw(GL_LINES, MAJOR);

  const glm::vec2 axes[4]{{-NUM_HALF_MAJOR_LINES * majorSpacing + ipos.x, 0},
                          {+NUM_HALF_MAJOR_LINES * majorSpacing + ipos.x, 0},
                          {0, -NUM_HALF_MAJOR_LINES * majorSpacing + ipos.y},
                          {0, +NUM_HALF_MAJOR_LINES * majorSpacing + ipos.y}};
  VBO_4->write(axes);
  SHADERS.line.setFragColor(BLACK)
      .setThickness(0.01f / MAIN_CAMERA.zoom())
      .draw(GL_LINES, VBO_4);
}

void WorldFrame::drawBezier(const Bezier &curve, const Color c0, const Color c1,
                            const float thickness, const bool debug) {
  static constexpr auto step = [](const float zoom, const glm::vec2 size) {
    const float s = std::max(size.x, size.y);
    return static_cast<unsigned int>(
        std::max(1.0f, s * std::min(std::sqrt(zoom), 2.0f) * 15.0f));
  };

  BaseFrame::drawBezier(curve, c0, c1, thickness, step, true, debug);
}
void WorldFrame::drawBezier(const Bezier &curve, const Color color,
                            const float thickness, const bool debug) {
  drawBezier(curve, color, color, thickness, debug);
}