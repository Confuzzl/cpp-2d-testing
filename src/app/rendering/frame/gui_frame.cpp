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
import uniform;
import math;
import aabb;
import ubo;
import fbo;

static glm::vec4 offsets() {
  return {random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f),
          random_float(-2.0f, 2.0f), random_float(-5.0f, 5.0f)};
}
static glm::vec2 point(const unsigned int i) {
  static glm::vec4 o[]{offsets(), offsets(), offsets(), offsets()};
  const double t = glfwGetTime();
  return {(sin(o[i][0] * t + o[i][1]) + 1) * App::WIDTH * 0.5,
          (sin(o[i][2] * t + o[i][3]) + 1) * App::HEIGHT * 0.5};
}

GUIFrame::GUIFrame() { matrix = App::UI_MATRIX; }

void GUIFrame::render() {
<<<<<<< HEAD
  shaders::getUBO<shaders::uniform::ViewBlock>().update({matrix});
=======
  shaders::uniformBlock<shaders::uniform::ViewBlock>({matrix});

  // drawBezier({point(0), point(1), point(2), point(3)}, colors::GREEN,
  //            colors::YELLOW, 5.0f, true);

  // debug();
>>>>>>> 897504e5eb9f49af32b14b9c1c6d0ff23dc2d678

  text(std::format("     {:>10}ns", MAIN_RENDERER.elapsed), BLACK, 0, 0);
  text(std::format("     {:>10.5}ms", MAIN_RENDERER.elapsed / 1'000'000.0),
       BLACK, 0, 30);
  text(std::format("min: {:>10.5}ms", MAIN_RENDERER.minElapsed / 1'000'000.0),
       BLACK, 0, 60);
  text(std::format("avg: {:>10.5}ms", (MAIN_RENDERER.elapsedAccumulate /
                                       MAIN_RENDERER.elapsedCounter) /
                                          1'000'000.0),
       BLACK, 0, 90);
  text(std::format("max: {:>10.5}ms", MAIN_RENDERER.maxElapsed / 1'000'000.0),
       BLACK, 0, 120);
  text(std::format("zoom: {:.1f}x2^{}", MAIN_CAMERA.zoomFraction(),
                   static_cast<int>(std::log2(MAIN_CAMERA.zoomExponent()))),
       BLACK, 0, 150);
  text(std::format("pos: {}", vec_string(MAIN_CAMERA.getPos())), BLACK, 0, 180);
}

void GUIFrame::debug() {
  static constexpr BoundingBox SCREEN{App::DIMENSIONS};
  VBO_4->write(SCREEN.toTriStrip());
  SHADERS.debug.draw(GL_TRIANGLE_STRIP, VBO_4);
}

void GUIFrame::drawBezier(const Bezier &curve, const Color c0, const Color c1,
                          const float thickness, const bool debug) {
  static constexpr auto step = [](const float, const glm::vec2 size) {
    const float s = std::max(size.x, size.y);
    return static_cast<unsigned int>(std::sqrt(s));
  };
  BaseFrame::drawBezier(curve, c0, c1, thickness, step, false, debug);
}
void GUIFrame::drawBezier(const Bezier &curve, const Color color,
                          const float thickness, const bool debug) {
  drawBezier(curve, color, color, thickness, debug);
}

namespace font {
constexpr unsigned short TEXEL_RANGE = 1 << 15;
constexpr unsigned short IMG_WIDTH = 1024, IMG_HEIGHT = 1024;
constexpr unsigned char CHAR_WIDTH = 64, CHAR_HEIGHT = 128;
constexpr unsigned char COLUMNS = 16, ROWS = 8;
} // namespace font

static constexpr unsigned short charWidthConvert(const unsigned char w) {
  return static_cast<unsigned short>(static_cast<float>(w) * font::CHAR_WIDTH /
                                     font::IMG_WIDTH * font::TEXEL_RANGE);
}
static constexpr unsigned short charHeightConvert(const unsigned char h) {
  return static_cast<unsigned short>(static_cast<float>(h) * font::CHAR_HEIGHT /
                                     font::IMG_HEIGHT * font::TEXEL_RANGE);
}
void GUIFrame::text(const std::string &str, const Color color,
                    const unsigned short x, const unsigned short y,
                    const float scale) const {
  static constexpr auto MAX_LENGTH = 0x100u;
  static constexpr glm::u16vec2 QUAD_UVS[2][3]{{{0, 0}, {1, 0}, {1, 1}},
                                               {{0, 0}, {1, 1}, {0, 1}}};
  static VBOHandle CHAR_VBO = VBO_HOLDER.get<vertex_layout::postex>(MAX_LENGTH);

  if (str.size() > MAX_LENGTH)
    return;

  const auto vertexCount = 6 * str.size();

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
    uvInfo *= TEXEL_RANGE;

    const glm::u16vec2 uvCoordinates{uvInfo.x, uvInfo.y};
    const glm::u16vec2 uvBoundingBoxs{uvInfo.z, uvInfo.w};

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
    SHADERS.texcol.setFragColor(color)
        .bindTexture(tex<"consolas1024.png">())
        .draw(GL_TRIANGLES, CHAR_VBO);
  } else {
    SHADERS.sdf.setFragColor(color)
        .setFontSize(scale)
        .bindTexture(tex<"sdf1024.png">())
        .draw(GL_TRIANGLES, CHAR_VBO);
  }
}
