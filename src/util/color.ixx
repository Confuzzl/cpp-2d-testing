module;

#include <limits>

export module color;

import debug;

import glm;
import math;
import <random>;

export struct Color {
  unsigned char r = 0, g = 0, b = 0, a = 255;

  constexpr Color() = default;
  template <typename T>
  constexpr Color(const T r, const T g, const T b, const T a)
      : r{static_cast<unsigned char>(r)}, g{static_cast<unsigned char>(g)},
        b{static_cast<unsigned char>(b)}, a{static_cast<unsigned char>(a)} {}

  template <typename T>
  constexpr Color(const T r, const T g, const T b) : Color(r, g, b, T{255}) {}

  template <std::integral T>
  constexpr Color(const T c)
      : Color(c >> 24 & 0xff, c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff) {}
  // template <typename T> constexpr Color(const T c) : Color(c, c, c) {}

  Color(const glm::vec3 &c) : Color(c.r, c.g, c.b) {}
  Color(const glm::vec4 &c) : Color(c.r, c.g, c.b, c.a) {}

  operator glm::vec3() const { return {r / 255.0f, g / 255.0f, b / 255.0f}; }
  operator glm::vec4() const {
    return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
  }
  constexpr operator unsigned int() const {
    return r << 24 | g << 16 | b << 8 | a;
  }

  Color &setRed(const unsigned char r) {
    this->r = r;
    return *this;
  }
  Color &setGreen(const unsigned char g) {
    this->g = g;
    return *this;
  }
  Color &setBlue(const unsigned char b) {
    this->b = b;
    return *this;
  }
  Color &setAlpha(const unsigned char a) {
    this->a = a;
    return *this;
  }
};

export constexpr Color operator""_gray(const unsigned long long n) {
  const unsigned char c = n & 0xff;
  return {c, c, c};
}
export constexpr Color operator""_rgb(const unsigned long long n) {
  const unsigned int c = n & 0xff'ff'ff;
  return {c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff};
}
export constexpr Color operator""_rgba(const unsigned long long n) {
  const unsigned int c = n & 0xff'ff'ff'ff;
  return {c};
}

export {
  const Color BLACK = 0x00_gray, DARKEST_GRAY = 0x1f_gray,
              DARK_GRAY = 0x3f_gray;
  const Color DARKISH_GRAY = 0x5f_gray, GRAY = 0x7f_gray,
              LIGHTISH_GRAY = 0x9f_gray;
  const Color LIGHT_GRAY = 0xbf_gray, LIGHTEST_GRAY = 0xdf_gray,
              WHITE = 0xff_gray;

  const Color RED = 0xff'00'00_rgb, LIGHT_RED = 0xff'7f'7f_rgb,
              DARK_RED = 0x7f'00'00_rgb;
  const Color GREEN = 0x00'ff'00_rgb, LIGHT_GREEN = 0x7f'ff'7f_rgb,
              DARK_GREEN = 0x00'7f'00_rgb;
  const Color BLUE = 0x00'00'ff_rgb, LIGHT_BLUE = 0x7f'7f'ff_rgb,
              DARK_BLUE = 0x00'00'7f_rgb;

  const Color CYAN = 0x00'ff'ff_rgb, LIGHT_CYAN = 0x7f'ff'ff_rgb,
              DARK_CYAN = 0x00'7f'7f_rgb;
  const Color MAGENTA = 0xff'00'ff_rgb, LIGHT_MAGENTA = 0xff'7f'ff_rgb,
              DARK_MAGENTA = 0x7f'00'7f_rgb;
  const Color YELLOW = 0xff'ff'00_rgb, LIGHT_YELLOW = 0xff'ff'7f_rgb,
              DARK_YELLOW = 0x7f'7f'00_rgb;

  const Color ROSE = 0xff'00'7f_rgb, ORANGE = 0xff'7f'00_rgb,
              LIME = 0x7f'ff'00_rgb;
  const Color SPRING_GREEN = 0x00'ff'7f_rgb, AZURE = 0x00'7f'ff_rgb,
              PURPLE = 0x7f'00'ff_rgb;
};

export namespace colors {
constexpr Color mix(const Color a, const Color b, const float t = 0.5) {
  return {(1 - t) * a.r + b.r * t, (1 - t) * a.g + b.g * t,
          (1 - t) * a.b + b.b * t, (1 - t) * a.a + b.a * t};
}

Color random() {
  return {random_int(0u, 255u), random_int(0u, 255u), random_int(0u, 255u)};
}

Color random_i(const unsigned long long i) {
  static std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(i));
  const auto num = gen();
  return {num | 0xff};
}
} // namespace colors