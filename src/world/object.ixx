module;

#include "util/gl.h"

export module object;

import glm;
import collision;
import mesh;

export namespace world {
struct BaseObject {
  enum ColliderType { CIRCLE, POLYGON };

private:
  collision::Collider *collider;
  ColliderType type;

  float mass;
  glm::vec2 velocity, acceleration;

  float angMass;
  float angVelocity, angAcceleration;

  float friction = 0;

  Mesh mesh;

public:
  BaseObject(collision::Collider *collider, const ColliderType type,
             Mesh &&mesh)
      : collider{collider}, type{type}, mesh{std::move(mesh)} {}

  void remove() {}

  virtual collision::Collider *operator->() { return collider; }
  virtual const collision::Collider *operator->() const { return collider; }

  ColliderType getType() const { return type; }
  Mesh &getMesh() { return mesh; }
  const Mesh &getMesh() const { return mesh; }
};
template <typename T> struct Object : BaseObject {
  std::unique_ptr<T> collider;

  T *operator->() override { return collider.get(); }
  const T *operator->() const override { return collider.get(); }
};
struct Circle : Object<collision::Circle> {};
struct Polygon : Object<collision::Polygon> {};

} // namespace world

namespace collision {
template <typename A> bool checkSecond(const A &a, const world::BaseObject &b) {
  switch (b.getType()) {
  case world::BaseObject::ColliderType::CIRCLE:
    return query(a, *static_cast<const world::Circle &>(b).collider);
  case world::BaseObject::ColliderType::POLYGON:
    return query(a, *static_cast<const world::Polygon &>(b).collider);
  }
  throw std::runtime_error{"INVALID OBJECT COLLISION QUERY"};
}
} // namespace collision
export namespace collision {
bool query(const world::BaseObject &a, const world::BaseObject &b) {
  switch (a.getType()) {
  case world::BaseObject::ColliderType::CIRCLE:
    return checkSecond(*static_cast<const world::Circle &>(a).collider, b);
  case world::BaseObject::ColliderType::POLYGON:
    return checkSecond(*static_cast<const world::Polygon &>(a).collider, b);
  }
  throw std::runtime_error{"INVALID OBJECT COLLISION QUERY"};
}
} // namespace collision
