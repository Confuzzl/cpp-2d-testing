module;

#include "util/gl.h"

export module world_frame;

import frame;
import glm;
import color;

export struct Mesh;

export struct WorldFrame : BaseFrame {
  WorldFrame();

  void render() override;

  struct render_opts {
    bool showAABB = false;
    GLenum primitive = GL_TRIANGLES;
  };
  void drawMesh(const Mesh &mesh, const glm::vec2 &pos, const float rot,
                const color_t &color, const GLenum primitive) const;

  void drawGrid() const;
};