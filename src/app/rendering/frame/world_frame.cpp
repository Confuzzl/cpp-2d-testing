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

  drawGrid();

  for (const auto [id, foo] : ECS.viewComponents<ecs::DirectRenderable>()) {
    foo->draw(this);
  }
  for (const auto [id, rend] : ECS.viewComponents<ecs::Renderable>()) {
    drawMesh(rend->mesh);
  }

  // drawBezier({point(0), point(1), point(2), point(3)}, colors::MAGENTA,
  //            colors::CYAN, 0.01f, true);
  //
  drawBezier({{0, 0}, {0, 1}, {1, 1}, {2, 0}}, colors::BLUE, 0.01f, true);
  drawBezier({{2, 0}, {3, -1}, {3, 0}, {3, 1}}, colors::RED, 0.01f, true);

  static const auto spline = Spline::begin({{-3, 0}, {-3, 1}, {-2, 1}, {-1, 0}})
                                 .add({0, 0}, {0, 1})
                                 .add({-1, 2}, {-2, 0})
      /*.endLoop()*/;
  for (auto i = 0u; i < spline.pieces.size(); i++) {
    float t = static_cast<float>(i) / spline.pieces.size();
    const Color a = colors::mix(colors::MAGENTA, colors::YELLOW, t);
    const Color b =
        colors::mix(colors::MAGENTA, colors::YELLOW,
                    static_cast<float>(i + 1) / spline.pieces.size());
    drawBezier(spline.pieces[i], a, b);
  }
}

void WorldFrame::drawGrid() const {
  static constexpr auto SPACING = 1.0f;
  static constexpr auto HALF_SIZE = 10;
  static constexpr auto WIDTH = 1.0f;
  static constexpr auto AXIS_COUNT = 2 * (HALF_SIZE * 2 + 1);
  static constexpr auto VERTEX_COUNT = 2 * AXIS_COUNT;
  static auto VBO = VBO_HOLDER.get<>(VERTEX_COUNT);
  static const glm::vec2 AXES[]{{1.0f, 0.0f}, {0.0f, 1.0f}};

  for (auto a = 0; a < 2; a++) {
    const glm::vec2 axis = AXES[a], perp = AXES[1 - a];
    for (auto line = -HALF_SIZE; line <= +HALF_SIZE; line++) {
      const glm::vec2 perpOffset = perp * static_cast<float>(line) * SPACING;

      VBO->write(-axis * static_cast<float>(HALF_SIZE) + perpOffset);
      VBO->write(+axis * static_cast<float>(HALF_SIZE) + perpOffset);
    }
  }

  SHADERS.basic.setView(matrix).setFragColor(colors::GRAY).draw(GL_LINES, VBO);
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