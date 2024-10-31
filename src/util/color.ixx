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
  template <typename T> constexpr Color(const T c) : Color(c, c, c) {}
  constexpr Color(const unsigned int c)
      : Color(c >> 24 & 0xff, c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff) {}
  Color(const glm::vec3 &c) : Color(c.r, c.g, c.b) {}
  Color(const glm::vec4 &c) : Color(c.r, c.g, c.b, c.a) {}

  operator glm::vec3() const { return {r / 255.0f, g / 255.0f, b / 255.0f}; }
  operator glm::vec4() const {
    return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
  }
  constexpr operator unsigned int() const {
    return r << 24 | g << 16 | b << 8 | a;
  }
};

// export constexpr Color operator""_rgb(const unsigned long long n) {
//   const unsigned int c = n & 0xff'ff'ff;
//   return {c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff};
// }
// export constexpr Color operator""_rgba(const unsigned long long n) {
//   return Color{static_cast<unsigned int>(n)};
// }

export namespace colors {
constexpr Color mix(const Color a, const Color b, const float t = 0.5) {
  return {(1 - t) * a.r + b.r * t, (1 - t) * a.g + b.g * t,
          (1 - t) * a.b + b.b * t, (1 - t) * a.a + b.a * t};
}

constexpr Color BLACK{0}, WHITE{255};
constexpr Color GRAY = mix(BLACK, WHITE), DARK_GRAY = mix(BLACK, GRAY),
                LIGHT_GRAY = mix(WHITE, GRAY);
constexpr Color RED{255, 0, 0}, GREEN{0, 255, 0}, BLUE{0, 0, 255};
constexpr Color MAGENTA{255, 0, 255}, YELLOW{255, 255, 0}, CYAN{0, 255, 255};

Color random() {
  return {random_int(0u, 255u), random_int(0u, 255u), random_int(0u, 255u)};
}

Color random_i(const unsigned int i) {
  static std::mt19937 gen;
  gen.seed(i);
  const auto num = gen();

  return {num | 0xff};
}
} // namespace colors