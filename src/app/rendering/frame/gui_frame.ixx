export module gui_frame;

import frame;
import <string>;
import color;
import glm;

namespace font {
constexpr unsigned short TEXEL_RANGE = 1 << 15;
constexpr unsigned short IMG_WIDTH = 1024, IMG_HEIGHT = 1024;
constexpr unsigned char CHAR_WIDTH = 64, CHAR_HEIGHT = 128;
constexpr unsigned char COLUMNS = 16, ROWS = 8;
} // namespace font

export struct GUIFrame : BaseFrame {
  GUIFrame();

  void render() override;

  void text(const std::string &str, const color_t &color = colors::WHITE,
            const unsigned short x = 0, const unsigned short y = 0,
            const double scale = 0.25) const;
};