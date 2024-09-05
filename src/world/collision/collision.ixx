module;

#include <ranges>

export module collision;

import glm;
import aabb;
import runtime_array;
import <vector>;
import <algorithm>;

export namespace collision {
struct Transformable {
  glm::vec2 position;
  float rotation;

  Transformable(const glm::vec2 position = {}, const float rotation = 0)
      : position{position}, rotation{rotation} {};
};
struct Collider {
protected:
  glm::vec2 position;
  float rotation;
  BoundingBox aabb;

  virtual void handleRotation() = 0;

public:
  Collider(const Transformable &t, BoundingBox &&aabb)
      : position{t.position}, rotation{t.rotation}, aabb{std::move(aabb)} {};

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

  void handleRotation() override {}

public:
  Circle(const Transformable &parent, const float radius)
      : Collider(parent,
                 {{parent.position - radius}, {parent.position + radius}}),
        radius{radius} {}

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

  using VertexArray = runtime_array<glm::vec2>;
  using EdgeArray = runtime_array<Edge>;

private:
  std::size_t count;
  VertexArray vertices;
  EdgeArray edges;

  Polygon(const Transformable &t, std::vector<glm::vec2> &&vertices);

  void handleRotation() override;

  glm::vec2 transform(const glm::vec2 v) const {
    const float sin = std::sinf(rotation), cos = std::cosf(rotation);
    return glm::vec2{v.x * cos - v.y * sin, v.x * sin + v.y * cos} + position;
  }

  struct VertexView {
    const Polygon *parent;
    const VertexArray *vertices;

    VertexView(const Polygon *parent, const VertexArray *vertices)
        : parent{parent}, vertices{vertices} {}

    struct iterator {
      const VertexView *view;
      std::size_t index;

      glm::vec2 operator*() const {
        return view->parent->transform((*(view->vertices))[index]);
      }
      constexpr bool operator==(const iterator &o) const {
        return index == o.index;
      }
      iterator &operator++() {
        index++;
        return *this;
      }
    };

    iterator begin() const { return {this, 0}; }
    iterator end() const { return {this, vertices->size()}; }
    glm::vec2 operator[](const unsigned int i) const {
      return *iterator{this, i};
    }

    std::size_t size() const { return vertices->size(); }
  } vertexView;

public:
  static Polygon from(const Transformable &t, std::vector<glm::vec2> &&vertices);
  static Polygon fromUnchecked(const Transformable &t,
                               std::vector<glm::vec2> &&vertices);

  const VertexView &getVertices() const { return vertexView; }
  const EdgeArray &getEdges() const { return edges; }
};

template <typename T> bool query(const T &a, const T &b);
template <typename A, typename B>
bool query(const A &a, const B &b, const bool reverse = false) {
  return query(b, a, true);
}
template <> bool query(const Circle &a, const Circle &b);
template <> bool query(const Polygon &a, const Polygon &b);
template <> bool query(const Polygon &a, const Circle &b, const bool reverse);

struct Resolution {
  glm::vec2 a{}, b{};

  Resolution() = default;
  Resolution(const glm::vec2 a, const glm::vec2 b) : a{a}, b{b} {}
  Resolution(const std::pair<glm::vec2, glm::vec2> &pair)
      : a{pair.first}, b{pair.second} {}

  // true if collision
  operator bool() const { return a.x * a.y * b.x * b.y; }
};

enum AABB_CHECK : bool { FALSE, TRUE };

template <AABB_CHECK check = TRUE, typename T>
Resolution resolve(const T &a, const T &b);
template <AABB_CHECK check = TRUE, typename A, typename B>
Resolution resolve(const A &a, const B &b, const bool reverse = false) {
  resolve<check>(b, a, true);
}
template <AABB_CHECK check = TRUE>
Resolution resolve(const Circle &a, const Circle &b);
template <AABB_CHECK check = TRUE>
Resolution resolve(const Polygon &a, const Polygon &b);
template <AABB_CHECK check = TRUE>
Resolution resolve(const Polygon &a, const Circle &b, const bool reverse);

} // namespace collision