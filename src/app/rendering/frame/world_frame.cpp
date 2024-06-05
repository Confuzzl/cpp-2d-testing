module;

#include "util/gl.h"

module world_frame;

import app;
import mesh;
import glm;
import vertices;
import buffer_objects;
import shaders;
import math;

// const glm::mat4 &world_frame::matrix() const { return MAIN_CAMERA.getView();
// }

WorldFrame::WorldFrame() : BaseFrame(glm::mat4{1.0f}) {}

void WorldFrame::render() {
  matrix = MAIN_CAMERA.getView();

  drawGrid();

  drawPoint({0, 0});

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
  GLintptr offset = 0;
  for (const vertex::simple &vertex : mesh.localVertexData) {
    glNamedBufferSubData(mesh.vbo.ID, offset, sizeof(vertex),
                         glm::value_ptr(vertex.pos));
    offset += sizeof(vertex);
  }

  shaders::shape.use(mesh.vbo, mesh.ebo);
  shaders::shape.setParentPos(pos)
      .setRotation(rot)
      .setView(matrix)
      .setFragColor(color);

  glDrawElements(primitive, mesh.ebo.count, GL_UNSIGNED_BYTE, 0);
}

void WorldFrame::drawGrid() const {
  static constexpr int HALF_SIZE = 10;
  static constexpr float WIDTH = 1;
  static constexpr unsigned int AXIS_COUNT = 2 * (HALF_SIZE * 2 + 1);
  static constexpr unsigned int VERTEX_COUNT = AXIS_COUNT * AXIS_COUNT;
  static vbo<vertex::simple> VBO{VERTEX_COUNT};
  static const glm::vec2 AXES[]{{1.0f, 0.0f}, {0.0f, 1.0f}};

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

  shaders::basic.use(VBO);
  shaders::basic.setView(matrix).setFragColor(colors::GRAY);

  glDrawArrays(GL_LINES, 0, VERTEX_COUNT);
}