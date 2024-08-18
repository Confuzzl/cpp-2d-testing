export module sat;

import collision;
import math;
import glm;
import <vector>;

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
  bool intersecting() const { return depth() < 0; }
};

namespace DepthFunction {
namespace Min {
constexpr float INITIAL_DEPTH = +F_INF;
constexpr bool compare(const float a, const float b) { return a < b; }
} // namespace Min
namespace Max {}
constexpr float INITIAL_DEPTH = -F_INF;
constexpr bool compare(const float a, const float b) { return b < a; }
} // namespace DepthFunction
using namespace DepthFunction::Min;

struct DepthInfo {
  const Polygon::Edge *edge;
  Axis axis;

  float depth() const { return axis.depth(); }
};

struct QueryInfo {
  glm::vec2 direction;
  float depth = INITIAL_DEPTH;

  auto resolution() const {
    return std::make_pair<glm::vec2, glm::vec2>(direction * INITIAL_DEPTH, {});
  }

  operator bool() const { return depth != INITIAL_DEPTH; }
};

// template <typename A, typename B> QueryInfo query(const A &a, const B &b);

enum PROJECTION_STATE : bool { NONE, INTERSECTION };
PROJECTION_STATE projectToDepths(const Polygon &a, const Polygon &b,
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
}
QueryInfo query(const Polygon &a, const Polygon &b) {
  std::vector<DepthInfo> depths;
  depths.reserve(a.getEdges().size() + b.getEdges().size());

  if (const PROJECTION_STATE ab = projectToDepths(a, b, depths); !ab)
    return {};
  if (const PROJECTION_STATE ba = projectToDepths(b, a, depths); !ba)
    return {};

  std::ranges::sort(depths, &compare, &DepthInfo::depth);
  const DepthInfo &best = depths[0];
  return {.direction = best.edge->parent == &a ? +best.edge->normal()
                                               : -best.edge->normal(),
          .depth = best.depth()};
}
} // namespace SAT
} // namespace collision