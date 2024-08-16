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
struct DepthInfo {
  const Polygon::Edge *edge;
  Axis axis;

  float depth() const { return axis.depth(); }
};

struct QueryInfo {
  glm::vec2 a, b;
};

// template <typename A, typename B> QueryInfo query(const A &a, const B &b);

enum struct PROJECTION_STATE : bool { NONE, INTERSECTION };
PROJECTION_STATE projectToDepths(const Polygon &a, const Polygon &b,
                                 std::vector<DepthInfo> &depths) {
  for (const auto &edge : a.getEdges()) {
    Axis axis{edge.normal()};
    for (const auto v : a.getVertices()) {
    }
  }
  return PROJECTION_STATE::INTERSECTION;
}
QueryInfo query(const Polygon &a, const Polygon &b) {
  std::vector<DepthInfo> depths;
  depths.reserve(a.getEdges().size() + b.getEdges().size());
  return {};
}
std::vector<DepthInfo> depths;
} // namespace SAT
} // namespace collision