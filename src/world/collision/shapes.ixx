export module shapes;

import glm;
import runtime_array;
import <variant>;
import <ranges>;

template <typename T> static auto transformWith(const T &transform) {
  return std::views::transform([&transform](const glm::vec2 p) {
    return ::transform(p, transform.position, transform.rotation);
  });
}

export namespace collision {
struct CollisionData {
  glm::vec2 a, b;

  glm::vec2 vec() const { return b - a; }
  glm::vec2 normal() const { return glm::normalize(vec()); }
  float length() const { return glm::length(vec()); }
  operator bool() const { return vec() != glm::vec2{0}; }
};

struct Wall {
private:
  union {
    struct {
      glm::vec2 a, b;
    };
    glm::vec2 vertices[2];
  };
  glm::vec2 _normal;

public:
  Wall(const glm::vec2 a, const glm::vec2 b)
      : a{a}, b{b}, _normal{glm::normalize(cw_perp(b - a))} {}

  template <typename T> auto vertexTransform(const T &transform) const {
    return vertices | transformWith(transform);
  }
  template <typename T> glm::vec2 normal(const T &transform) const {
    return ::transform(_normal, {}, transform.rotation);
  }
};
struct Circle {
  float radius;
};
struct Polygon {
  runtime_array<glm::vec2> vertices;
};
struct Collider {
  std::variant<Wall, Circle, Polygon> data;

  template <typename T> bool is() const {
    return std::holds_alternative<T>(data);
    ;
  }
  template <typename T> T &to() { return std::get<T>(data); }
  template <typename T> const T &to() const { return std::get<T>(data); }

  template <typename W, typename C, typename P>
  auto visit(W &&w, C &&c, P &&p) const {
    if (is<Wall>())
      return w(to<Wall>());
    if (is<Circle>())
      return c(to<Circle>());
    return p(to<Polygon>());
  }
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