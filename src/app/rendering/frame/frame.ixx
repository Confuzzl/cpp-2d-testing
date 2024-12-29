module;

#include "util/gl.h"

export module frame;

import glm;
import color;
import collision;
// import vertex_layout;
import texture;
import mesh;
import bezier;

export struct BoundingBox;

export struct BaseFrame {
  glm::mat4 matrix{1.0f};

  void drawPoint(const glm::vec2 point, const float size = 10,
                 const Color color = BLACK) const;
  void drawPointFixed(const glm::vec2 point, const float size = 10,
                      const Color color = BLACK) const;

  void drawLinePerspective(const BoundingBox &dimensions, const float thickness,
                           const Color color = BLACK) const;
  void drawLineConstant(const BoundingBox &dimensions, const float thickness,
                        const Color color = BLACK) const;
  void drawLine(const BoundingBox &dimensions, const Color color = BLACK) const;

  void drawArrow(const BoundingBox &dimensions,
                 const Color color = BLACK) const;

  void drawCircle(const glm::vec2 center, const float radius,
                  const Color color = BLACK) const;

  void drawBoxPerspective(const BoundingBox &dimensions, const float thickness,
                          const Color color = BLACK) const;
  void drawBoxConstant(const BoundingBox &dimensions, const float thickness,
                       const Color color = BLACK) const;

  void drawBox(const BoundingBox &dimensions, const Color color = BLACK) const;

  void drawQuad(const BoundingBox &dimensions, const Color color = BLACK) const;

  void drawTexture(const BoundingBox &dimensions, const GL::Texture &texture);

  void drawBoxBlur(const BoundingBox &dimensions, const GL::Texture &texture,
                   const unsigned int radius, const unsigned int direction);
  void drawBoxBlur(const BoundingBox &dimensions, const GL::Texture &texture,
                   const unsigned int radius);

  void drawOutline(const BoundingBox &dimensions, const GL::Texture &texture,
                   const unsigned int radius, const Color color);

  void drawMesh(const Mesh &mesh, const glm::vec2 &pos = {},
                const float rot = 0) const;

  void drawBezier(const Bezier &curve, const Color c0, const Color c1,
                  const float thickness,
                  unsigned int (*stepFunction)(const float, const glm::vec2),
                  const bool world, const bool debug);
};