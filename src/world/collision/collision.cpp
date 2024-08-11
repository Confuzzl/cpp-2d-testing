module collision;

import glm;

using namespace collision;

Polygon::Polygon(geometry::Polygon &&polygon)
    : geometry::Polygon(std::move(polygon)) {}
Polygon Polygon::from(geometry::Polygon &&polygon) {
  // https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex

  const auto &vertices = polygon.getVertices();
  const auto size = vertices.size();

  bool positive = false;
  for (auto i = 0; i < size; i++) {
    const glm::vec2 a = vertices[i];
    const glm::vec2 b = vertices[(i + 1) % size];
    const glm::vec2 c = vertices[(i + 2) % size];
    const float z = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);

    if (i == 0)
      positive = z > 0;
    else if (positive != (z > 0))
      throw std::runtime_error{"POLYGON IS NOT CONVEX"};
  }
  return fromUnchecked(std::move(polygon));
}
Polygon Polygon::fromUnchecked(geometry::Polygon &&polygon) {
  return {std::move(polygon)};
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
