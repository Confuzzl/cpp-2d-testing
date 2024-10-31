module;

#include "util/gl.h"
#include "util/main_objects.h"
#include <cmath>
#include <limits.h>

module gui_frame;

import app;
import rendering;
import glm;
import vertex_layout;
import shaders;
import texture;
import input_handler;
import <vector>;
import <format>;
import debug;
import bo_heap;

import math;

static glm::vec4 offsets() {
  return {random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f),
          random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f)};
}
static glm::vec2 point(const unsigned int i) {
  static glm::vec4 o[]{offsets(), offsets(), offsets(), offsets()};
  const float t = glfwGetTime();
  return {(sin(o[i][0] * t + o[i][1]) + 1) * App::WIDTH * 0.5,
          (sin(o[i][2] * t + o[i][3]) + 1) * App::HEIGHT * 0.5};
}

void GUIFrame::render() {
  matrix = Renderer::UI_MATRIX;

  text(std::format("{:>8.4}ms", MAIN_RENDERER.elapsed / 1'000'000.0),
       colors::BLACK);
  text(std::format("{:>8}ns", MAIN_RENDERER.elapsed), colors::BLACK, 0, 30);

  glm::vec2 P0 = point(0);
  glm::vec2 P1 = point(1);
  glm::vec2 P2 = point(2);
  glm::vec2 P3 = point(3);
  // glm::vec2 P0{50, 50};
  // glm::vec2 P1{150, 250};
  // glm::vec2 P2{500, 300};
  // glm::vec2 P3{600, 100};

  static const glm::vec2 quad[] = {
      {0, 0}, {App::WIDTH, 0}, {0, App::HEIGHT}, {App::WIDTH, App::HEIGHT}};
  VBO_4->write(quad);
  SHADERS.guiBezier.setView(matrix)
      .setColor(colors::BLUE, colors::GREEN)
      .setPoints(P0, P1, P2, P3)
      .draw(GL_TRIANGLE_STRIP, VBO_4);
}

static constexpr unsigned short charWidthConvert(const unsigned char w) {
  return static_cast<unsigned short>(static_cast<float>(w) * font::CHAR_WIDTH /
                                     font::IMG_WIDTH * font::TEXEL_RANGE);
}
static constexpr unsigned short charHeightConvert(const unsigned char h) {
  return static_cast<unsigned short>(static_cast<float>(h) * font::CHAR_HEIGHT /
                                     font::IMG_HEIGHT * font::TEXEL_RANGE);
}
void GUIFrame::text(const std::string &str, const Color &color,
                    const unsigned short x, const unsigned short y,
                    const float scale) const {
  static constexpr auto MAX_LENGTH = 0x100u;
  static const glm::lowp_u16vec2 QUAD_UVS[2][3]{{{0, 0}, {1, 0}, {1, 1}},
                                                {{0, 0}, {1, 1}, {0, 1}}};
  static VBOHandle CHAR_VBO = VBO_HOLDER.get<vertex_layout::postex>(MAX_LENGTH);

  if (str.size() > MAX_LENGTH)
    return;

  const unsigned int vertexCount = 6 * static_cast<unsigned int>(str.size());

  std::vector<vertex_layout::postex> vertices{};
  vertices.reserve(vertexCount);

  unsigned short xOffset = x;
  for (const char c : str) {
    const char id = std::min(c - 32, 96);
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
        const glm::vec2 pos{xOffset + width * QUAD_UVS[tri][v].x,
                            y + height * QUAD_UVS[tri][v].y};
        const auto uv = QUAD_UVS[tri][v];
        vertices.emplace_back(
            pos.x, pos.y, charWidthConvert(column) + uv.x * charWidthConvert(1),
            charHeightConvert(font::ROWS - row - 1) +
                uv.y * charHeightConvert(1));
      }
    }

    xOffset += static_cast<unsigned short>(font::CHAR_WIDTH * scale);
  }

  CHAR_VBO->write(vertices);

  if (scale < 1) {
    SHADERS.texcol.setView(matrix)
        .setFragColor(color)
        .bindTexture(TEXTURE.font)
        .draw(GL_TRIANGLES, CHAR_VBO);
  } else {
    SHADERS.sdf.setView(matrix)
        .setFragColor(color)
        .setFontSize(scale)
        .bindTexture(TEXTURE.sdfFont)
        .draw(GL_TRIANGLES, CHAR_VBO);
  }
}