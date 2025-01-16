export module shapes;

import glm;
import runtime_array;

export namespace collision {
struct CollisionData {
  glm::vec2 a, b;

  // #define CACHE_DATA
  // #ifndef CACHE_DATA
  glm::vec2 normal() const { return glm::normalize(b - a); }
  float length() const { return glm::length(b - a); }
  // #else
  //   glm::vec2 normal_;
  //   float _length;
  //
  //   glm::vec2 normal() const { return normal; }
  //   float length() const { return _length; }
  // #endif

  operator bool() const { return b - a != glm::vec2{0}; }
};

struct Wall {
  glm::vec2 a, b;
  glm::vec2 normal;

  Wall(const glm::vec2 a, const glm::vec2 b)
      : a{a}, b{b}, normal{cw_perp(b - a)} {}
};
struct Circle {
  float radius;
};
struct Polygon {
  runtime_array<glm::vec2> vertices;
};

CollisionData push(const Circle &c1, const glm::vec2 p1, const Circle &c2,
                   const glm::vec2 p2) {
  const auto rsum = c1.radius + c2.radius;
  const auto diff = p2 - p1;
  if (glm::dot(diff, diff) > rsum * rsum)
    return {};
  const auto diffN = glm::normalize(diff);
  const auto a = p1 + diffN * c1.radius, b = p2 - diffN * c2.radius;
  return {.a{a}, .b{b}};
}
} // namespace collision