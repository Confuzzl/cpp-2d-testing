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
    const Polygon *parent = nullptr;
    unsigned int tail = 0, head = 0;

    Edge() = default;
    Edge(const Polygon *parent, const unsigned int tail,
         const unsigned int head);

    glm::vec2 getTail() const;
    glm::vec2 getHead() const;

    operator glm::vec2() const;
    glm::vec2 normal() const;
  };

private:
  unsigned int count;
  std::unique_ptr<glm::vec2[]> vertices;
  std::unique_ptr<Edge[]> edges;

  Polygon(Collider &&parent, std::vector<glm::vec2> &&vertices);

  void handleRotation() override;

  glm::vec2 transform(const glm::vec2 v) const {
    const float sin = std::sinf(rotation), cos = std::cosf(rotation);
    return glm::vec2{v.x * cos - v.y * sin, v.x * sin + v.y * cos} + position;
  }

  struct VertexView {
    const Polygon *parent;
    std::span<glm::vec2> span;

    VertexView(const Polygon *parent)
        : parent{parent}, span{parent->vertices.get(), parent->count} {}

    struct iterator {
      const VertexView *view;
      unsigned int index;

      glm::vec2 operator*() const {
        return view->parent->transform(view->span[index]);
      }
      constexpr bool operator==(const iterator &o) const {
        return index == o.index;
      }
      iterator operator++() const { return {view, index + 1}; }
    };

    iterator begin() const { return {this, 0}; }
    iterator end() const {
      return {this, static_cast<unsigned int>(span.size())};
    }
    glm::vec2 operator[](const unsigned int i) const {
      return *iterator{this, i};
    }
  } vertexView;

public:
  static Polygon from(Collider &&parent, std::vector<glm::vec2> &&vertices);
  static Polygon fromUnchecked(Collider &&parent,
                               std::vector<glm::vec2> &&vertices);

  const VertexView &getVertices() const { return vertexView; }
  std::span<Edge> getEdges() const { return {edges.get(), count}; }
};

template <typename T> bool colliding(const T &a, const T &b);
template <typename A, typename B> bool colliding(const A &a, const B &b) {
  return colliding(b, a);
}

template <> bool colliding<Circle>(const Circle &a, const Circle &b);
template <> bool colliding<Polygon>(const Polygon &a, const Polygon &b);

template <> bool colliding<Circle, Polygon>(const Circle &a, const Polygon &b);
} // namespace collision