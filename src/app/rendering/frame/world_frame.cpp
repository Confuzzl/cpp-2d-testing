module;

#include "util/gl.h"

module world_frame;

import app;
import mesh;
import glm;
import vertices;
import buffer_object;
import shaders;
import math;

WorldFrame::WorldFrame() : BaseFrame(glm::mat4{1.0f}) {}

void WorldFrame::render() {
  matrix = MAIN_CAMERA.getView();

  drawGrid();

  // drawQuad({{0, 0}, {1, 1}}, colors::RED);
  drawCircle({0, 0}, 0.5, colors::BLUE);

  // for (int i = 0; i < 10; i++) {
  //   drawPoint(random_vec({-2, -2}, {+2, +2}), random_float(0.0f, 1.0f),
  //             colors::random());
  // }

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

void WorldFrame::drawMesh(const Mesh &mesh, const glm::vec2 &pos,
                          const float rot, const color_t &color,
                          const GLenum primitive) const {
  // GLintptr offset = 0;
  // for (const vertex::simple &vertex : mesh.localVertexData) {
  //   glNamedBufferSubData(mesh.vbo.ID, offset, sizeof(vertex),
  //                        glm::value_ptr(vertex.pos));
  //   offset += sizeof(vertex);
  // }

  // shaders::shape.use(mesh.vbo, mesh.ebo);
  // shaders::shape.setParentPos(pos)
  //     .setRotation(rot)
  //     .setView(matrix)
  //     .setFragColor(color);

  // glDrawElements(primitive, mesh.ebo.count, GL_UNSIGNED_BYTE, 0);
}

void WorldFrame::drawGrid() const {
  static constexpr int HALF_SIZE = 10;
  static constexpr float WIDTH = 1;
  static constexpr unsigned int AXIS_COUNT = 2 * (HALF_SIZE * 2 + 1);
  static constexpr unsigned int VERTEX_COUNT = 2 * AXIS_COUNT;
  static VBOHandle VBO = VBO_HOLDER.get<vertex::simple>(VERTEX_COUNT);
  static const glm::vec2 AXES[]{{1.0f, 0.0f}, {0.0f, 1.0f}};

  for (auto a = 0; a < 2; a++) {
    const glm::vec2 axis = AXES[a], perp = AXES[1 - a];
    for (auto line = -HALF_SIZE; line <= +HALF_SIZE; line++) {
      const glm::vec2 perpOffset = perp * static_cast<float>(line);

      VBO.write(axis * static_cast<float>(-HALF_SIZE) + perpOffset);
      VBO.write(axis * static_cast<float>(+HALF_SIZE) + perpOffset);
    }
  }

  shaders::basic.setView(matrix).setFragColor(colors::GRAY);
  shaders::basic.draw(GL_LINES, VBO);
}