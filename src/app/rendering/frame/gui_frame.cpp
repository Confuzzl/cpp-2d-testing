module;

#include "util/gl.h"
#include <limits.h>

module gui_frame;

import glm;
import vertices;
// import buffer_objects;
import rendering;
import shaders;
import texture;
import input_handler;
import <vector>;
import <format>;

GUIFrame::GUIFrame() : BaseFrame(Renderer::UI_MATRIX) {}

void GUIFrame::render() {
  text(std::format("{:>8.4}ms", MAIN_RENDERER.elapsed / 1'000'000.0),
       colors::BLACK);
  text(std::format("{:>8}ns", MAIN_RENDERER.elapsed), colors::BLACK, 0, 50);
}

static unsigned short charWidthConvert(const unsigned char w) {
  return static_cast<unsigned short>(static_cast<float>(w) * font::CHAR_WIDTH /
                                     font::IMG_WIDTH * font::TEXEL_RANGE);
}
static unsigned short charHeightConvert(const unsigned char h) {
  return static_cast<unsigned short>(static_cast<float>(h) * font::CHAR_HEIGHT /
                                     font::IMG_HEIGHT * font::TEXEL_RANGE);
}
void GUIFrame::text(const std::string &str, const color_t &color,
                    const unsigned short x, const unsigned short y,
                    const double scale) const {
  static constexpr unsigned int MAX_LENGTH = 0xff;
  static constexpr GLushort QUAD_UVS[2][3][2]{{{0, 0}, {1, 0}, {1, 1}},
                                              {{0, 0}, {1, 1}, {0, 1}}};
  static VBOHandle CHAR_VBO = VBO_HOLDER.get<vertex::font>(MAX_LENGTH);

  // static const glm::lowp_u16vec2 QUAD_UVS[2][3]{{{0, 0}, {1, 0}, {1, 1}},
  //                                               {{0, 0}, {1, 1}, {0, 1}}};

  const unsigned int vertexCount = 6 * static_cast<unsigned int>(str.size());

  std::vector<vertex::font> vertices{};
  vertices.reserve(vertexCount);

  unsigned short xOffset = x;
  for (const char c : str) {
    const char id = c - 32;
    const unsigned char row = id / font::COLUMNS, column = id % font::COLUMNS;

    const double width = font::CHAR_WIDTH * scale,
                 height = font::CHAR_HEIGHT * scale;

    glm::vec4 uvInfo{column * font::CHAR_WIDTH, row * font::CHAR_HEIGHT,
                     font::CHAR_WIDTH, font::CHAR_HEIGHT};
    uvInfo[1] = font::IMG_HEIGHT - uvInfo[0] - uvInfo[3];
    uvInfo /= glm::vec4{font::IMG_WIDTH, font::IMG_HEIGHT, font::IMG_WIDTH,
                        font::IMG_HEIGHT};
    uvInfo *= glm::vec4{SHRT_MAX};

    const glm::lowp_u16vec2 uvCoordinates{uvInfo[0], uvInfo[1]};
    const glm::lowp_u16vec2 uvDimensions{uvInfo[2], uvInfo[3]};

    for (auto tri = 0; tri < 2; tri++) {
      for (auto v = 0; v < 3; v++) {
        const glm::vec2 pos{xOffset + width * QUAD_UVS[tri][v][0],
                            y + height * QUAD_UVS[tri][v][1]};
        const GLushort *uv = QUAD_UVS[tri][v];
        vertices.emplace_back(pos[0], pos[1],
                              charWidthConvert(column) +
                                  uv[0] * charWidthConvert(1),
                              charHeightConvert(font::ROWS - row - 1) +
                                  uv[1] * charHeightConvert(1));
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