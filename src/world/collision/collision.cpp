module;

#include <stdexcept>

module collision;

import glm;

using namespace collision;

Polygon::Edge::Edge(const Polygon *view, const unsigned int tail,
                    const unsigned int head)
    : view{view}, tail{tail}, head{head} {}
glm::vec2 Polygon::Edge::getTail() const { return view->getVertices()[tail]; }
glm::vec2 Polygon::Edge::getHead() const { return view->getVertices()[head]; }
Polygon::Edge::operator glm::vec2() const { return getHead() - getTail(); }
glm::vec2 Polygon::Edge::normal() const { return ccw_perp(*this); }

static std::unique_ptr<glm::vec2[]>
verticesHelper(std::vector<glm::vec2> &&vertices) {
  auto out = std::make_unique<glm::vec2[]>(vertices.size());
  std::move(vertices.begin(), vertices.end(), out.get());
  return out;
}
static std::unique_ptr<Polygon::Edge[]> edgesHelper(const Polygon *polygon,
                                                    const unsigned int count) {
  auto out = std::make_unique<Polygon::Edge[]>(count);
  for (auto i = 0u; i < count; i++)
    out.get()[i] = Polygon::Edge{polygon, i, (i + 1) % count};
  return out;
}
Polygon::Polygon(Collider &&view, std::vector<glm::vec2> &&vertices)
    : Collider(std::move(view)),
      count{static_cast<unsigned int>(vertices.size())},
      vertices{verticesHelper(std::move(vertices))},
      edges{edgesHelper(this, count)}, vertexView{this} {}
Polygon Polygon::from(Collider &&view, std::vector<glm::vec2> &&vertices) {
  // https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex

  const auto size = vertices.size();

  for (auto i = 0u; i < size; i++) {
    const glm::vec2 a = vertices[i];
    const glm::vec2 b = vertices[(i + 1) % size];
    const glm::vec2 c = vertices[(i + 2) % size];
    const float z = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);

    if (z < 0)
      throw std::runtime_error{"POLYGON IS NOT CONVEX"};
  }
  return fromUnchecked(std::move(view), std::move(vertices));
}
Polygon Polygon::fromUnchecked(Collider &&view,
                               std::vector<glm::vec2> &&vertices) {
  return {std::move(view), std::move(vertices)};
}

void Polygon::handleRotation() {
  aabb.reset();
  for (const auto v : getVertices())
    aabb.expand(v);
}

import sat;

namespace collision {
template <> bool colliding<Circle>(const Circle &a, const Circle &b) {
  const float r = a.getRadius() + b.getRadius();
  return glm::distance2(a.getPos(), b.getPos()) <= r * r;
}
template <> bool colliding<Polygon>(const Polygon &a, const Polygon &b) {
  using namespace SAT;
  return query(a, b);
}

template <> bool colliding<Circle, Polygon>(const Circle &a, const Polygon &b) {
  return false;
}
} // namespace collision
