export module gui_frame;

import frame;
import <string>;
import color;
import glm;
import bezier;

export struct GUIFrame : BaseFrame {
  GUIFrame();

  void render();

  void text(const std::string &str, const Color color = BLACK,
            const unsigned short x = 0, const unsigned short y = 0,
            const float scale = 0.25) const;

  // void drawLine(const BoundingBox &BoundingBoxs, const float size,
  //               const Color color = BLACK) const;

  void drawBezier(const Bezier &curve, const Color c0, const Color c1,
                  const float thickness = 5.0f, const bool debug = false);
  void drawBezier(const Bezier &curve, const Color color,
                  const float thickness = 5.0f, const bool debug = false);

  void debug();
};