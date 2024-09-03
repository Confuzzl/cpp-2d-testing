export module polycirc;

import glm;
import collision;
import collision_generic;
import <algorithm>;

export namespace collision {
namespace poly_circ {
bool query(const Polygon &a, const Circle &b) {
  if (!a.getAABB().intersects(b.getAABB()))
    return false;
  if (pointInCircle(a.getPos(), b))
    return true;
  for (const auto &e : a.getEdges())
    if (edgeIntersectsCircle(e.getTail(), e.getHead(), b))
      return true;
  if (pointInPolygon(b.getPos(), a))
    return true;
  return false;
}
std::pair<glm::vec2, glm::vec2> resolve(const Polygon &a, const Circle &b,
                                        const bool reverse) {
  const auto aBox = a.getAABB(), bBox = b.getAABB();
  if (!aBox.intersects(bBox))
    return {};

  if (pointInCircle(a.getPos(), b)) {

    if (reverse) {
      // push circle out of polygon
    } else {
      // push polygon out of circle
    }

    const auto minPushBack = a.getPos() - b.getPos();
    const auto maxPushBack = glm::normalize(minPushBack);
    return {};
  }

  struct Info {
    glm::vec2 edgePoint, circlePoint;
    float _d2;

    float d2() const { return _d2; }
  };
  std::vector<Info> depths;
  for (const auto &e : a.getEdges()) {
    const auto [edgePoint, circlePoint, intersection] =
        edgeQueryCircle(e.getTail(), e.getHead(), b);
    if (intersection)
      depths.emplace_back(edgePoint, circlePoint,
                          glm::distance2(edgePoint, circlePoint));
  }
  std::ranges::sort(depths, std::less<float>{}, &Info::d2);

  if (pointInPolygon(b.getPos(), a))
    return {};
  return {};
}
} // namespace poly_circ
} // namespace collision