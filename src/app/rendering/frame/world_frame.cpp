module;

#include "util/gl.h"
#include "util/main_objects.h"
#include <variant>

module world_frame;

import app;
import camera;
import mesh;
import glm;
import vertex_layout;
import bo_heap;
import shaders;
import math;
import ecs_component;
import <random>;

static glm::vec4 offsets() {
  return {random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f),
          random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f)};
}
static glm::vec2 point(const unsigned int i) {
  static glm::vec4 o[]{offsets(), offsets(), offsets(), offsets()};
  const float t = glfwGetTime();
  return {sin(o[i][0] * t + o[i][1]), sin(o[i][2] * t + o[i][3])};
}

void WorldFrame::render() {
  matrix = MAIN_CAMERA.getView();

  drawGrid();

  for (const auto [id, foo] : ECS.viewComponents<ecs::DirectRenderable>()) {
    foo->func(this);
  }
  for (const auto [id, rend] : ECS.viewComponents<ecs::Renderable>()) {
    drawMesh(rend->mesh);
  }

  glm::vec2 P0 = point(0);
  glm::vec2 P1 = point(1);
  glm::vec2 P2 = point(2);
  glm::vec2 P3 = point(3);

  static const glm::vec2 quad[] = {{-10, -10}, {10, -10}, {-10, 10}, {10, 10}};
  VBO_4->write(quad);
  SHADERS.worldBezier.setView(matrix)
      .setScreenDimensions({App::WIDTH, App::HEIGHT})
      .setColor(colors::RED, colors::YELLOW)
      .setPoints(P0, P1, P2, P3)
      .draw(GL_TRIANGLE_STRIP, VBO_4);
}

void WorldFrame::drawGrid() const {
  static constexpr auto HALF_SIZE = 10;
  static constexpr auto WIDTH = 1.f;
  static constexpr auto AXIS_COUNT = 2 * (HALF_SIZE * 2 + 1);
  static constexpr auto VERTEX_COUNT = 2 * AXIS_COUNT;
  static auto VBO = VBO_HOLDER.get<>(VERTEX_COUNT);
  static const glm::vec2 AXES[]{{1.0f, 0.0f}, {0.0f, 1.0f}};

  for (auto a = 0; a < 2; a++) {
    const glm::vec2 axis = AXES[a], perp = AXES[1 - a];
    for (auto line = -HALF_SIZE; line <= +HALF_SIZE; line++) {
      const glm::vec2 perpOffset = perp * static_cast<float>(line);

      VBO->write(-axis * static_cast<float>(HALF_SIZE) + perpOffset);
      VBO->write(+axis * static_cast<float>(HALF_SIZE) + perpOffset);
    }
  }

  SHADERS.basic.setView(matrix).setFragColor(colors::GRAY).draw(GL_LINES, VBO);
}