export module collision;

import world.geometry;

import <vector>;

export namespace collision {
struct Circle : geometry::Circle {};
struct Polygon : geometry::Polygon {
  bool isConvex;

  Polygon(geometry::Polygon &&polygon, const bool isConvex = false);
};
struct ConvexPolygon : Polygon {
  struct Edge {
    const glm::vec2 *tail, const *head;

    Edge(const glm::vec2 &tail, const glm::vec2 &head)
        : tail{&tail}, head{&head} {}

    operator glm::vec2() const { return *head - *tail; }
  };

  ConvexPolygon(geometry::Polygon &&polygon);

  static ConvexPolygon from(geometry::Polygon &&polygon);
  static ConvexPolygon fromUnchecked(geometry::Polygon &&polygon);
};

template <typename T> bool colliding(const T &a, const T &b);
template <typename A, typename B> bool colliding(const A &a, const B &b);

template <> bool colliding<Circle>(const Circle &a, const Circle &b);
template <> bool colliding<Polygon>(const Polygon &a, const Polygon &b);
template <>
bool colliding<ConvexPolygon>(const ConvexPolygon &a, const ConvexPolygon &b);

template <> bool colliding<Circle, Polygon>(const Circle &a, const Polygon &b);
template <>
bool colliding<Circle, ConvexPolygon>(const Circle &a, const ConvexPolygon &b);
template <>
bool colliding<Polygon, ConvexPolygon>(const Polygon &a,
                                       const ConvexPolygon &b);

template <> bool colliding<Polygon, Circle>(const Polygon &a, const Circle &b) {
  return colliding(b, a);
}
template <>
bool colliding<ConvexPolygon, Circle>(const ConvexPolygon &a, const Circle &b) {
  return colliding(b, a);
}
template <>
bool colliding<ConvexPolygon, Polygon>(const ConvexPolygon &a,
                                       const Polygon &b) {
  return colliding(b, a);
}
} // namespace collision