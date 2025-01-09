export module aabb;

import glm;
import math;
import poly;
import <array>;
import <vector>;
import debug;

export struct BoundingBox {
  glm::vec2 min{+F_INF, +F_INF}, max{-F_INF, -F_INF};

  constexpr BoundingBox() = default;
  constexpr BoundingBox(const glm::vec2 min, const glm::vec2 max)
      : min{min}, max{max} {}
  constexpr BoundingBox(const glm::vec2 max) : BoundingBox({}, max) {}

  constexpr bool intersects(const BoundingBox &other) const {
    return (min.x < other.max.x && max.x > other.min.x) &&
           (min.y < other.max.y && max.y > other.min.y);
  }
  constexpr bool contains(const glm::vec2 point) const {
    return in_range(point.x, min.x, max.x) && in_range(point.y, min.y, max.y);
  }

  constexpr void expand(const float padding) {
    min -= padding;
    max += padding;
  }
  constexpr void expand(const glm::vec2 p) {
    min.x = std::min(min.x, p.x);
    max.x = std::max(max.x, p.x);
    min.y = std::min(min.y, p.y);
    max.y = std::max(max.y, p.y);
  }
  constexpr void expand(const BoundingBox &other) {
    expand(other.min);
    expand(other.max);
  }

  constexpr BoundingBox operator+(const glm::vec2 v) const {
    return {min + v, max + v};
  }
  constexpr BoundingBox &operator+=(const glm::vec2 v) {
    min += v;
    max += v;
    return *this;
  }
  constexpr BoundingBox operator-(const glm::vec2 v) const {
    return {min - v, max - v};
  }
  constexpr BoundingBox &operator-=(const glm::vec2 v) {
    min -= v;
    max -= v;
    return *this;
  }

  constexpr float height() const { return max.y - min.y; }
  constexpr float width() const { return max.x - min.x; }
  constexpr glm::vec2 size() const { return {width(), height()}; }
  constexpr float area() const { return width() * height(); }
  constexpr glm::vec2 median() const { return min + size() / 2.0f; }

  constexpr std::array<glm::vec2, 4> toTriStrip() const {
    // .y union access cant be constexpr?
    return {{min, {max.x, min.y}, {min.x, max.y}, max}};
  }
  constexpr std::array<glm::vec2, 4> toLineLoop() const {
    // .y union access cant be constexpr?
    return {{min, {max.x, min.y}, max, {min.x, max.y}}};
  }

  constexpr void reset() {
    min = {+F_INF, +F_INF};
    max = {-F_INF, -F_INF};
  }

  static constexpr BoundingBox startSize(const glm::vec2 start,
                                         const glm::vec2 size) {
    return {start, start + size};
  }
};

import <format>;

export template <> struct std::formatter<BoundingBox> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
  auto format(const BoundingBox &aabb, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "[{}, {}]", vec_string(aabb.min),
                          vec_string(aabb.max));
  }
};