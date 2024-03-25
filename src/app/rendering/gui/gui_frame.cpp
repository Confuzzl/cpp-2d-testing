module;

#include "util/gl.h"
#include <limits.h>

module gui_frame;

import glm;
import vertices;
import buffer_objects;
import rendering;
import shader;
import texture;
import input_handler;

import <format>;

gui::frame::frame() {}
gui::frame::~frame() = default;

void gui::frame::render() const {}

static unsigned short charWidthConvert(const unsigned char w) {
  return static_cast<unsigned short>(
      static_cast<float>(w) * gui::font::CHAR_WIDTH / gui::font::IMG_WIDTH *
      gui::font::TEXEL_RANGE);
}
static unsigned short charHeightConvert(const unsigned char h) {
  return static_cast<unsigned short>(
      static_cast<float>(h) * gui::font::CHAR_HEIGHT / gui::font::IMG_HEIGHT *
      gui::font::TEXEL_RANGE);
}
void gui::frame::text(const std::string &str, const unsigned short x,
                      const unsigned short y, const double scale) const {
  static const glm::lowp_u16vec2 QUAD_UVS[2][3]{{{0, 0}, {1, 0}, {1, 1}},
                                                {{0, 0}, {1, 1}, {0, 1}}};

  const GLuint vertexCount = 6 * static_cast<GLuint>(str.size());

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
        const glm::lowp_u16vec2 uv = QUAD_UVS[tri][v];
        vertices.emplace_back(pos[0], pos[1],
                              charWidthConvert(column) +
                                  uv[0] * charWidthConvert(1),
                              charHeightConvert(font::ROWS - row - 1) +
                                  uv[1] * charHeightConvert(1));
      }
    }

    xOffset += static_cast<unsigned short>(font::CHAR_WIDTH * scale);
  }

  vbo<vertex::font> vbo{vertexCount};

  shader::font.use(vbo);

  GLintptr offset = 0;
  for (const auto &vertex : vertices) {
    glNamedBufferSubData(vbo.ID, offset, sizeof(vertex.pos),
                         glm::value_ptr(vertex.pos));
    offset += sizeof(vertex.pos);
    glNamedBufferSubData(vbo.ID, offset, sizeof(vertex.tex),
                         glm::value_ptr(vertex.tex));
    offset += sizeof(vertex.tex);
  }
  shader::font.setView(Renderer::UI_MATRIX).setFontColor(colors::WHITE);
  glBindTextureUnit(0, tex::font.ID);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void gui::frame::drawQuad(const unsigned short x, const unsigned short y,
                          const unsigned short width,
                          const unsigned short height,
                          const color_t &color) const {
  drawQuadFromTo(x, y, x + width, y + height, color);
}

void gui::frame::drawQuadFromTo(const unsigned short x1,
                                const unsigned short y1,
                                const unsigned short x2,
                                const unsigned short y2,
                                const color_t &color) const {
  static vbo<vertex::simple> VBO{4};
  static simple_ebo EBO{{0, 1, 2, 0, 2, 3}};
  const glm::vec2 corners[4] = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};
  shader::basic.use(VBO, EBO);

  GLintptr offset = 0;
  for (const glm::vec2 &corner : corners) {
    glNamedBufferSubData(VBO.ID, offset, sizeof(corner),
                         glm::value_ptr(corner));
    offset += sizeof(corner);
  }

  shader::basic.setView(Renderer::UI_MATRIX).setFragColor(color);

  glDrawElements(GL_TRIANGLES, EBO.count, GL_UNSIGNED_BYTE, 0);
}