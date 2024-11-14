export module world_frame;

import frame;
import glm;
import color;
import bezier;

export struct WorldFrame : BaseFrame {
  void render();

  void drawGrid() const;

  // void drawLinePerspective(const BoundingBox &BoundingBoxs, const float
  // thickness,
  //                          const Color color) const;
  //  void drawLineConstant(const BoundingBox &BoundingBoxs, const float
  //  thickness,
  //                        const Color color) const;

  void drawBezier(const Bezier &curve, const Color c0, const Color c1,
                  const float thickness = 0.01f, const bool debug = false);
  void drawBezier(const Bezier &curve, const Color color,
                  const float thickness = 0.01f, const bool debug = false);
};