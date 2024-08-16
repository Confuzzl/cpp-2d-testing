module;

#include <ranges>

export module collision;

import glm;
import aabb;
import <vector>;
import <algorithm>;

export namespace collision {
struct Collider {
protected:
  glm::vec2 position;
  float rotation;
  BoundingBox aabb;

  virtual void handleRotation() = 0;

public:
  Collider(const glm::vec2 position = {}, const float rotation = 0)
      : position{position}, rotation{rotation} {};

  glm::vec2 getPos() const { return position; }
  void setPos(const glm::vec2 v) { position = v; }
  void translate(const glm::vec2 v) { setPos(position + v); }
  float getRotation() const { return rotation; }
  void setRot(const float r) {
    rotation = r;
    handleRotation();
  }
  void rotate(const float r) { setRot(rotation + r); }

  BoundingBox getAABB() const { return aabb + position; }
};

struct Circle : Collider {
private:
  float radius = 1;

public:
  Circle(Collider &&parent, const float radius)
      : Collider(std::move(parent)), radius{radius} {}

  float getRadius() const { return radius; }
};

// CCW and convex
struct Polygon : Collider {
  struct Edge {
    const Polygon *parent;
    unsigned int tail, head;

    Edge(const Polygon *parent, const unsigned int tail,
         const unsigned int head);
    operator glm::vec2() const;
    glm::vec2 normal() const;
  };

protected:
  std::vector<glm::vec2> vertices;
  std::vector<Edge> edges;

  Polygon(Collider &&parent, std::vector<glm::vec2> &&vertices);

private:
  void handleRotation() override;

  // auto newVertexView() {
  //   return vertices |
  //          std::views::transform(
  //              [position = this->position, rotation = this->rotation](
  //                  const glm::vec2 v) { return position + v; });
  // }

public:
  static Polygon from(Collider &&parent, std::vector<glm::vec2> &&vertices);
  static Polygon fromUnchecked(Collider &&parent,
                               std::vector<glm::vec2> &&vertices);

  auto getVertices() const {
    // return vertices | std::ranges::transform([]);
    return vertices | std::views::transform([position = this->position,
                                             rotation = this->rotation](
                                                const glm::vec2 v) {
             const float sin = std::sinf(rotation), cos = std::cosf(rotation);
             return glm::vec2{v.x * cos - v.y * sin, v.x * sin + v.y * cos} +
                    position;
           });
  }
  const auto &getEdges() const { return edges; }
};

template <typename T> bool colliding(const T &a, const T &b);
template <typename A, typename B> bool colliding(const A &a, const B &b) {
  return colliding(b, a);
}

template <> bool colliding<Circle>(const Circle &a, const Circle &b);
template <> bool colliding<Polygon>(const Polygon &a, const Polygon &b);

template <> bool colliding<Circle, Polygon>(const Circle &a, const Polygon &b);
} // namespace collision