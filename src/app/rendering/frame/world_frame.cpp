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

void WorldFrame::render() {
  matrix = MAIN_CAMERA.getView();

  drawGrid();

  // auto c = collision::Circle::from,{{}, 1.0f};
  // drawCircle(c, colors::RED);
  // auto p = collision::Polygon::from(
  //     {{}}, {{-0.5, -0.5}, {0.5, -0.5}, {0, 0.5}, {-0.5, 0.5}});
  // drawPolygon(p, colors::BLUE);

  // Mesh mesh{{{-1, -1}, {+1, -1}, {+2, +1}, {+0, +2}, {-2, +1}},
  //                  GL_TRIANGLE_FAN};
  // drawMesh(mesh);

  // for (const std::unique_ptr<base_obj_t> &obj : MAIN_SCENE.objs) {
  //   obj->draw();
  // }

  // for (const auto &o : MAIN_SCENE.objs2) {
  //   drawQuad({o.min, o.max}, colors::CYAN);
  // }

  // for (const auto &n : MAIN_SCENE.tree.nodes) {
  //   drawBox({n.box.min, n.box.max},
  //           static_cast<float>((MAIN_SCENE.tree.maxDepth - n.depth + 1) * 2),
  //           colors::random_i(n.depth));
  // }
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

  SHADERS.basic.setView(matrix).setFragColor(colors::GRAY);
  SHADERS.basic.draw(GL_LINES, VBO);
}