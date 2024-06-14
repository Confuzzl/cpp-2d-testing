module;

#include "util/gl.h"
#include <limits.h>

module gui_frame;

import glm;
import vertices;
import app;
import rendering;
import shaders;
import texture;
import input_handler;
import <vector>;
import <format>;
import debug;

GUIFrame::GUIFrame() : BaseFrame(Renderer::UI_MATRIX) {}

void GUIFrame::render() {
  text(std::format("{:>8.4}ms", MAIN_RENDERER.elapsed / 1'000'000.0),
       colors::BLACK);
  text(std::format("{:>8}ns", MAIN_RENDERER.elapsed), colors::BLACK, 0, 50);
}

static constexpr unsigned short charWidthConvert(const unsigned char w) {
  return static_cast<unsigned short>(static_cast<float>(w) * font::CHAR_WIDTH /
                                     font::IMG_WIDTH * font::TEXEL_RANGE);
}
static constexpr unsigned short charHeightConvert(const unsigned char h) {
  return static_cast<unsigned short>(static_cast<float>(h) * font::CHAR_HEIGHT /
                                     font::IMG_HEIGHT * font::TEXEL_RANGE);
}
void GUIFrame::text(const std::string &str, const color_t &color,
                    const unsigned short x, const unsigned short y,
                    const float scale) const {
  static constexpr unsigned int MAX_LENGTH = 0x100;
  // static constexpr GLushort QUAD_UVS[2][3][2]{{{0, 0}, {1, 0}, {1, 1}},
  //                                             {{0, 0}, {1, 1}, {0, 1}}};
  static const glm::lowp_u16vec2 QUAD_UVS[2][3]{{{0, 0}, {1, 0}, {1, 1}},
                                                {{0, 0}, {1, 1}, {0, 1}}};
  static VBOHandle CHAR_VBO = VBO_HOLDER.get<vertex::font>(MAX_LENGTH);

  // if (str.size() > MAX_LENGTH)
  //   return;

  const unsigned int vertexCount = 6 * static_cast<unsigned int>(str.size());

  std::vector<vertex::font> vertices{};
  vertices.reserve(vertexCount);

  unsigned short xOffset = x;
  for (const char c : str) {
    const char id = c - 32;
    const unsigned char row = id / font::COLUMNS, column = id % font::COLUMNS;

    const float width = font::CHAR_WIDTH * scale,
                height = font::CHAR_HEIGHT * scale;

    glm::vec4 uvInfo{column * font::CHAR_WIDTH, row * font::CHAR_HEIGHT,
                     font::CHAR_WIDTH, font::CHAR_HEIGHT};
    uvInfo.y = font::IMG_HEIGHT - uvInfo.x - uvInfo.z;
    uvInfo /= glm::vec4{font::IMG_WIDTH, font::IMG_HEIGHT, font::IMG_WIDTH,
                        font::IMG_HEIGHT};
    uvInfo *= glm::vec4{SHRT_MAX};

    const glm::lowp_u16vec2 uvCoordinates{uvInfo.x, uvInfo.y};
    const glm::lowp_u16vec2 uvDimensions{uvInfo.z, uvInfo.w};

    for (auto tri = 0; tri < 2; tri++) {
      for (auto v = 0; v < 3; v++) {
        const glm::vec2 pos{xOffset + width * QUAD_UVS[tri][v][0],
                            y + height * QUAD_UVS[tri][v][1]};
        const auto &uv = QUAD_UVS[tri][v];
        vertices.emplace_back(
            pos.x, pos.y, charWidthConvert(column) + uv.x * charWidthConvert(1),
            charHeightConvert(font::ROWS - row - 1) +
                uv.y * charHeightConvert(1));
      }
    }

    xOffset += static_cast<unsigned short>(font::CHAR_WIDTH * scale);
  }

  for (const vertex::font &vertex : vertices) {
    CHAR_VBO.writePartial(vertex.pos);
    CHAR_VBO.write(vertex.tex);
  }

  shaders::font.setView(matrix).setFragColor(color);
  glBindTextureUnit(0, tex::font.ID);
  shaders::font.draw(GL_TRIANGLES, CHAR_VBO);
}