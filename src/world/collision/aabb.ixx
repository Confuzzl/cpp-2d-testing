export module aabb;

import glm;
import math;
import poly;

import <vector>;

export struct BoundingBox {
  glm::vec2 min{+F_INF, +F_INF}, max{-F_INF, -F_INF};

  BoundingBox() = default;
  BoundingBox(const glm::vec2 min, const glm::vec2 max) : min{min}, max{max} {}

  bool intersects(const BoundingBox &other) const {
    return (min.x < other.max.x && max.x > other.min.x) &&
           (min.y < other.max.y && max.y > other.min.y);
  }
  bool contains(const glm::vec2 point) const {
    return in_range(point.x, min.x, max.x) && in_range(point.y, min.y, max.y);
  }

  // void expand(const glm::vec2 p) {
  //   min.x = std::min(min.x, p.x);
  //   max.x = std::max(max.x, p.x);
  //   min.y = std::min(min.y, p.y);
  //   max.y = std::max(max.y, p.y);
  // }
  // void expand(const BoundingBox &other) {
  //   expand(other.min);
  //   expand(other.max);
  // }

  BoundingBox operator+(const glm::vec2 v) const { return {min + v, max + v}; }
  BoundingBox &operator+=(const glm::vec2 v) {
    min += v;
    max += v;
    return *this;
  }
  BoundingBox operator-(const glm::vec2 v) const { return {min - v, max - v}; }
  BoundingBox &operator-=(const glm::vec2 v) {
    min -= v;
    max -= v;
    return *this;
  }

  float height() const { return max.y - min.y; }
  float width() const { return max.x - min.x; }
  glm::vec2 size() const { return {width(), height()}; }
  float area() const { return width() * height(); }
  glm::vec2 median() const { return min + size() / 2.0f; }
};

import <format>;

export template <> struct std::formatter<BoundingBox> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
  auto format(const BoundingBox &aabb, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "[{}, {}]", vec_string(aabb.min),
                          vec_string(aabb.max));
  }
};