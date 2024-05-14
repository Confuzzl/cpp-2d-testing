module;

#include "util/gl.h"

export module world_frame;

import glm;
import color;

import dimensions;

export struct Mesh;

export namespace world {
struct frame {
  void render() const;

  struct render_opts {
    bool showAABB = false;
    GLenum primitive = GL_TRIANGLES;
  };

  void drawMesh(const Mesh &mesh, const glm::vec2 &pos, const float rot,
                const color_t &color, const GLenum primitive) const;

  void drawGrid() const;

  void drawPoint(const glm::vec2 &point, const float size = 10,
                 const color_t &color = colors::WHITE) const;
  void drawPointFixed(const glm::vec2 &point, const float size = 10,
                      const color_t &color = colors::WHITE) const;

  void drawLine(const glm::vec2 &from, const glm::vec2 &to,
                const float size = 0.1,
                const color_t &color = colors::WHITE) const;
  void drawLineFixed(const glm::vec2 &from, const glm::vec2 &to,
                     const color_t &color = colors::WHITE) const;

  void drawArrow(const Dimensions &dimensions,
                 const color_t &color = colors::WHITE) const;

  enum struct RenderMode { SOLID, STRIPED, OUTLINE };

  void drawCircle(const glm::vec2 &center, const float radius,
                  const color_t &color = colors::WHITE) const;

  void drawBox(const Dimensions &dimensions, const float lineSize = 5,
               const color_t &color = colors::WHITE) const;
  void drawBoxFixed(const Dimensions &dimensions, const float lineSize = 5,
                    const color_t &color = colors::WHITE) const;
  void drawQuad(const Dimensions &dimensions,
                const color_t &color = colors::WHITE) const;
};
} // namespace world