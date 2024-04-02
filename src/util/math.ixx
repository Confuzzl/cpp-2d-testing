module;

#include <limits>

export module math;

import <random>;
import glm;

export template <typename T = float> T random_float(const T a, const T b) {
  static std::random_device rd;
  static std::mt19937 mt{rd()};

  return std::uniform_real_distribution<T>{a, b}(mt);
}

export template <typename T = int> T random_int(const T a, const T b) {
  static std::random_device rd;
  static std::mt19937 mt{rd()};

  return std::uniform_int_distribution<T>{a, b}(mt);
}

export template <typename T>
bool in_range(const T n, const T min, const T max) {
  return min <= n && n <= max;
}

export constexpr float F_INF = std::numeric_limits<float>::infinity();
export constexpr double D_INF = std::numeric_limits<double>::infinity();