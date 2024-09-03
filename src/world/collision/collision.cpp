module;

#include <stdexcept>

module collision;

import glm;

using namespace collision;

Polygon::Edge::Edge(const Polygon *parent, const unsigned int tail,
                    const unsigned int head)
    : parent{parent}, tail{tail}, head{head} {}
glm::vec2 Polygon::Edge::getTail() const { return parent->getVertices()[tail]; }
glm::vec2 Polygon::Edge::getHead() const { return parent->getVertices()[head]; }
Polygon::Edge::operator glm::vec2() const { return getHead() - getTail(); }
glm::vec2 Polygon::Edge::normal() const { return cw_perp(*this); }

static Polygon::EdgeArray edgesHelper(const Polygon *polygon,
                                      const std::size_t count) {
  auto out = std::make_unique<Polygon::Edge[]>(count);
  for (auto i = 0u; i < count; i++)
    out.get()[i] =
        Polygon::Edge{polygon, i, static_cast<unsigned int>((i + 1) % count)};
  return {std::move(out), count};
}
static BoundingBox aabbHelper(const std::vector<glm::vec2> &vertices) {
  BoundingBox out;
  for (const auto v : vertices)
    out.expand(v);
  return out;
}

Polygon::Polygon(Transformable &&t, std::vector<glm::vec2> &&vertices)
    : Collider(std::move(t), aabbHelper(vertices)), count{vertices.size()},
      vertices{make_runtime_array<glm::vec2>(std::move(vertices))},
      edges{std::move(edgesHelper(this, count))},
      vertexView{this, &this->vertices} {}
Polygon Polygon::from(Transformable &&t, std::vector<glm::vec2> &&vertices) {
  // https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex

  const auto size = vertices.size();

  if (size < 3)
    throw std::runtime_error("POLYGON HAS LESS THAN 3 VERTICES");

  for (auto i = 0u; i < size; i++) {
    const glm::vec2 a = vertices[i];
    const glm::vec2 b = vertices[(i + 1) % size];
    const glm::vec2 c = vertices[(i + 2) % size];
    const float z = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);

    if (z < 0)
      throw std::runtime_error{"POLYGON IS NOT CONVEX"};
  }
  return fromUnchecked(std::move(t), std::move(vertices));
}
Polygon Polygon::fromUnchecked(Transformable &&t,
                               std::vector<glm::vec2> &&vertices) {
  return {std::move(t), std::move(vertices)};
}

void Polygon::handleRotation() {
  aabb.reset();
  for (const auto v : getVertices())
    aabb.expand(v);
}

import sat;
import polycirc;

namespace collision {
template <> bool query(const Circle &a, const Circle &b) {
  const float r = a.getRadius() + b.getRadius();
  return glm::distance2(a.getPos(), b.getPos()) <= r * r;
}
template <> bool query(const Polygon &a, const Polygon &b) {
  using namespace SAT;
  return query(a, b);
}
template <> bool query(const Polygon &a, const Circle &b, const bool reverse) {
  using namespace poly_circ;
  return query(a, b);
}

template <> Resolution resolve(const Circle &a, const Circle &b) {
  const float r = a.getRadius() + b.getRadius();
  const float r2 = r * r;
  const float d2 = glm::distance2(a.getPos(), b.getPos());
  if (d2 >= r2)
    return {};
  // d2 < r2
  // d < r
  const float m = r - std::sqrt(d2);
  return {glm::normalize(a.getPos() - b.getPos()) * m, {}};
}
template <> Resolution resolve(const Polygon &a, const Polygon &b) {
  using namespace SAT;
  return {resolve(a, b)};
}
template <>
Resolution resolve(const Polygon &a, const Circle &b, const bool reverse) {
  return {};
}
} // namespace collision
