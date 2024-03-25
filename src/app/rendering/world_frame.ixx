module;

#include "util/gl.h"

export module world_frame;

import glm;
import color;

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

  void drawLine(const glm::vec2 &a, const glm::vec2 &b, const float size = 5,
                const color_t &color = colors::WHITE) const;
  void drawLineFixed(const glm::vec2 &a, const glm::vec2 &b,
                     const float size = 5,
                     const color_t &color = colors::WHITE) const;

  void drawArrow(const glm::vec2 &origin, const glm::vec2 &vector,
                 const color_t &color = colors::WHITE) const;
  void drawArrowFromTo(const glm::vec2 &from, const glm::vec2 &to,
                       const color_t &color = colors::WHITE) const;

  void drawCircle(const glm::vec2 &center, const float radius,
                  const color_t &color = colors::WHITE) const;

  void drawBox(const glm::vec2 &start, const glm::vec2 &size,
               const float lineSize = 5,
               const color_t &color = colors::WHITE) const;
  void drawBoxFromTo(const glm::vec2 &from, const glm::vec2 &to,
                     const float lineSize = 5,
                     const color_t &color = colors::WHITE) const;
  void drawBoxFixed(const glm::vec2 &start, const glm::vec2 &size,
                    const float lineSize = 5,
                    const color_t &color = colors::WHITE) const;
  void drawBoxFromToFixed(const glm::vec2 &from, const glm::vec2 &to,
                          const float lineSize = 5,
                          const color_t &color = colors::WHITE) const;

  void drawQuad(const glm::vec2 &start, const glm::vec2 &size,
                const color_t &color = colors::WHITE) const;
  void drawQuadFromTo(const glm::vec2 &from, const glm::vec2 &to,
                      const color_t &color = colors::WHITE) const;
};
} // namespace world