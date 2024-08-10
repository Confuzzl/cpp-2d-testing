export module collision;

import world.geometry;

import <vector>;

export namespace collision {
struct Circle : geometry::Circle {};
struct Polygon : geometry::Polygon {
  struct Edge {
    const glm::vec2 *tail, const *head;

    Edge(const glm::vec2 &tail, const glm::vec2 &head)
        : tail{&tail}, head{&head} {}

    operator glm::vec2() const { return *head - *tail; }

    glm::vec2 normal();
  };

  Polygon(geometry::Polygon &&polygon);

  static Polygon from(geometry::Polygon &&polygon);
  static Polygon fromUnchecked(geometry::Polygon &&polygon);
};

template <typename T> bool colliding(const T &a, const T &b);
template <typename A, typename B> bool colliding(const A &a, const B &b) {
  return colliding(b, a);
}

template <> bool colliding<Circle>(const Circle &a, const Circle &b);
template <> bool colliding<Polygon>(const Polygon &a, const Polygon &b);

template <> bool colliding<Circle, Polygon>(const Circle &a, const Polygon &b);
} // namespace collision