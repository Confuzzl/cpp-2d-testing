export module collision_generic;

import collision;
import glm;

export namespace collision {
bool pointInCircle(const glm::vec2 p, const Circle &circle) {
  const float r = circle.getRadius();
  return glm::distance2(p, circle.getPos()) <= r * r;
}
bool pointInPolygon(const glm::vec2 p, const Polygon &polygon) {
  for (const auto &e : polygon.getEdges()) {
    if (glm::dot(e.normal(), p - e.getTail()) > 0)
      return false;
  }
  return true;
}

bool edgeIntersectsCircle(const glm::vec2 a, const glm::vec2 b,
                          const Circle &circle) {
  // https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
  const auto ac = circle.getPos() - a;
  const auto abn = glm::normalize(b - a);
  const float dot = glm::dot(ac, abn);
  const auto d = a + abn * dot;
  return pointInCircle(d, circle);
}

auto edgeQueryCircle(const glm::vec2 a, const glm::vec2 b,
                     const Circle &circle) {
  struct Info {
    glm::vec2 edgePoint, circlePoint;
    bool intersection;
  };
  // https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
  const auto c = circle.getPos();
  const auto ac = c - a;
  const auto abn = glm::normalize(b - a);
  const float dot = glm::dot(ac, abn);
  const auto d = a + abn * dot;
  const auto e = c + glm::normalize(d - c) * circle.getRadius();
  return Info{d, e, pointInCircle(d, circle)};
}
} // namespace collision