module collision;

import glm;
import <stdexcept>;

using namespace collision;

Polygon::Edge::Edge(const Polygon *parent, const unsigned int tail,
                    const unsigned int head)
    : parent{parent}, tail{tail}, head{head} {}
Polygon::Edge::operator glm::vec2() const { return {}; }
glm::vec2 Polygon::Edge::normal() { return {}; }

Polygon::Polygon(Collider &&parent, std::vector<glm::vec2> &&vertices)
    : Collider(std::move(parent)), vertices{std::move(vertices)} {}
Polygon Polygon::from(Collider &&parent, std::vector<glm::vec2> &&vertices) {
  // https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex

  const auto size = vertices.size();

  for (auto i = 0; i < size; i++) {
    const glm::vec2 a = vertices[i];
    const glm::vec2 b = vertices[(i + 1) % size];
    const glm::vec2 c = vertices[(i + 2) % size];
    const float z = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);

    if (z < 0)
      throw std::runtime_error{"POLYGON IS NOT CONVEX"};
  }
  return fromUnchecked(std::move(parent), std::move(vertices));
}
Polygon Polygon::fromUnchecked(Collider &&parent,
                               std::vector<glm::vec2> &&vertices) {
  return {std::move(parent), std::move(vertices)};
}

namespace collision {
template <> bool colliding<Circle>(const Circle &a, const Circle &b) {
  const float r = a.getRadius() + b.getRadius();
  const float r2 = r * r;
  return glm::distance2(a.getPos(), b.getPos()) <= r2;
}
template <> bool colliding<Polygon>(const Polygon &a, const Polygon &b) {

  return false;
}

template <> bool colliding<Circle, Polygon>(const Circle &a, const Polygon &b) {
  return false;
}
} // namespace collision
