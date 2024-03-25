export module aabb;

import glm;
import math;
import poly;

import <vector>;

export struct aabb_t {
  glm::vec2 min{+F_INF, +F_INF}, max{-F_INF, -F_INF};

  aabb_t() = default;
  aabb_t(const glm::vec2 &min, const glm::vec2 &max) : min{min}, max{max} {}

  bool intersects(const aabb_t &other) const {
    return (min.x < other.max.x && max.x > other.min.x) &&
           (min.y < other.max.y && max.y > other.min.y);
  }
  bool contains(const glm::vec2 &point) const {
    return in_range(point.x, min.x, max.x) && in_range(point.y, min.y, max.y);
  }
  void expand(const glm::vec2 &p) {
    min.x = std::min(min.x, p.x);
    max.x = std::max(max.x, p.x);
    min.y = std::min(min.y, p.y);
    max.y = std::max(max.y, p.y);
  }
  void expand(const aabb_t &other) {
    expand(other.min);
    expand(other.max);
  }
  void translate(const glm::vec2 &v) {
    min += v;
    max += v;
  }

  float height() const { return max.y - min.y; }
  float width() const { return max.x - min.x; }
  glm::vec2 size() const { return {width(), height()}; }
  float area() const { return width() * height(); }
  glm::vec2 median() const { return min + size() / 2.0f; }
};