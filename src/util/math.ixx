module;

#include <limits>

export module math;

import <random>;

export template <std::floating_point T = float>
T random_float(const T a, const T b) {
  static std::random_device rd;
  static std::mt19937 mt{rd()};

  return std::uniform_real_distribution<T>{a, b}(mt);
}

export template <std::integral T = int>
T random_int(const T a = std::numeric_limits<T>::min(),
             const T b = std::numeric_limits<T>::max()) {
  static std::random_device rd;
  static std::mt19937 mt{rd()};

  return std::uniform_int_distribution<T>{a, b}(mt);
}

export template <typename T>
constexpr bool in_range(const T n, const T min, const T max) {
  return min <= n && n <= max;
}

export constexpr float F_INF = std::numeric_limits<float>::infinity();
export constexpr double D_INF = std::numeric_limits<double>::infinity();