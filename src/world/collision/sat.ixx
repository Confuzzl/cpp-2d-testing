export module sat;

import collision;
import math;
import glm;
import <vector>;
import <algorithm>;

import debug;

export namespace collision {
namespace SAT {
struct Axis {
  float minA = +F_INF, maxA = -F_INF;
  float minB = +F_INF, maxB = -F_INF;
  glm::vec2 direction;

  Axis(const glm::vec2 direction) : direction{glm::normalize(direction)} {}

private:
  void project(float &min, float &max, const glm::vec2 p) {
    const float dot = glm::dot(direction, p);
    min = std::min(min, dot);
    max = std::max(max, dot);
  }

public:
  void projectA(const glm::vec2 p) { project(minA, maxA, p); }
  void projectB(const glm::vec2 p) { project(minB, maxB, p); }

  float depth() const { return std::max(minA, minB) - std::min(maxA, maxB); }
  bool intersecting() const { return depth() <= 0; }
};

struct DepthInfo {
  const Polygon::Edge *edge;
  Axis axis;

  float depth() const { return axis.depth(); }
};

enum PROJECTION_STATE : bool { NONE, INTERSECTION };

template <AABB_CHECK check = TRUE>
bool query(const Polygon &a, const Polygon &b) {
  static constexpr auto project = [](const Polygon &a, const Polygon &b) {
    for (const auto &edge : a.getEdges()) {
      Axis axis{edge.normal()};
      for (const auto v : a.getVertices())
        axis.projectA(v);
      for (const auto v : b.getVertices())
        axis.projectB(v);
      if (!axis.intersecting())
        return PROJECTION_STATE::NONE;
    }
    return PROJECTION_STATE::INTERSECTION;
  };

  if constexpr (check)
    if (!a.getAABB().intersects(b.getAABB()))
      return false;
  if (const PROJECTION_STATE ab = project(a, b); !ab)
    return false;
  if (const PROJECTION_STATE ba = project(b, a); !ba)
    return false;
  return true;
}
template <AABB_CHECK check = TRUE>
std::pair<glm::vec2, glm::vec2> resolve(const Polygon &a, const Polygon &b) {
  static constexpr auto projectToDepths = [](const Polygon &a, const Polygon &b,
                                             std::vector<DepthInfo> &depths) {
    for (const auto &edge : a.getEdges()) {
      Axis axis{edge.normal()};
      for (const auto v : a.getVertices())
        axis.projectA(v);
      for (const auto v : b.getVertices())
        axis.projectB(v);
      if (!axis.intersecting())
        return PROJECTION_STATE::NONE;
      depths.emplace_back(&edge, std::move(axis));
    }
    return PROJECTION_STATE::INTERSECTION;
  };

  if constexpr (check)
    if (!a.getAABB().intersects(b.getAABB()))
      return {};

  std::vector<DepthInfo> depths;
  depths.reserve(a.getEdges().size() + b.getEdges().size());

  if (const PROJECTION_STATE ab = projectToDepths(a, b, depths); !ab)
    return {};
  if (const PROJECTION_STATE ba = projectToDepths(b, a, depths); !ba)
    return {};

  // infos have negative depth so compare negative or reverse comparison
  std::ranges::sort(depths, std::greater<float>{}, &DepthInfo::depth);
  const DepthInfo &best = depths[0];

  const glm::vec2 direction =
      (best.edge->parent == &a ? +1.0f : -1.0f) * best.edge->normal();
  return std::make_pair<glm::vec2, glm::vec2>(direction * best.depth(), {});
}
} // namespace SAT
} // namespace collision