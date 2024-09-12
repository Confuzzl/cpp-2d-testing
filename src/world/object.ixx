module;

#include "util/gl.h"

export module object;

import glm;
import collision;
import mesh;

export namespace world {
struct BaseObject {
  enum COLLIDER_TYPE { CIRCLE, POLYGON };

private:
  collision::Collider *collider;
  COLLIDER_TYPE type;

  float mass;
  glm::vec2 velocity, acceleration;

  float angMass;
  float angVelocity, angAcceleration;

  float friction = 0;

  Mesh mesh;

public:
  BaseObject(collision::Collider *collider, const COLLIDER_TYPE type,
             Mesh &&mesh)
      : collider{collider}, type{type}, mesh{std::move(mesh)} {}

  void remove() {}

  virtual collision::Collider *operator->() { return collider; }
  virtual const collision::Collider *operator->() const { return collider; }

  collision::Collider *getCollider() { return collider; }
  const collision::Collider *getCollider() const { return collider; }

  COLLIDER_TYPE getType() const { return type; }
  Mesh &getMesh() { return mesh; }
  const Mesh &getMesh() const { return mesh; }
};
template <typename T> struct Object : BaseObject {
  std::unique_ptr<T> collider;

  Object(std::unique_ptr<T> &&collider, const COLLIDER_TYPE type, Mesh &&mesh)
      : BaseObject(collider.get(), type, std::move(mesh)),
        collider{std::move(collider)} {}

  T *operator->() override { return collider.get(); }
  const T *operator->() const override { return collider.get(); }
};
struct Circle : Object<collision::Circle> {
  Circle(std::unique_ptr<collision::Circle> &&collider, Mesh &&mesh)
      : Object(std::move(collider), CIRCLE, std::move(mesh)) {}
};
struct Polygon : Object<collision::Polygon> {
  Polygon(std::unique_ptr<collision::Polygon> &&collider, Mesh &&mesh)
      : Object(std::move(collider), POLYGON, std::move(mesh)) {}
};

} // namespace world

namespace collision {
template <AABB_CHECK check = TRUE, typename A>
bool checkSecond(const A &a, const world::BaseObject &b) {
  switch (b.getType()) {
  case world::BaseObject::COLLIDER_TYPE::CIRCLE:
    return query<check>(a, *static_cast<const world::Circle &>(b).collider);
  case world::BaseObject::COLLIDER_TYPE::POLYGON:
    return query<check>(a, *static_cast<const world::Polygon &>(b).collider);
  }
  throw std::runtime_error{"INVALID OBJECT COLLISION QUERY"};
}
} // namespace collision
export namespace collision {
template <AABB_CHECK check = TRUE>
bool query(const world::BaseObject &a, const world::BaseObject &b) {
  switch (a.getType()) {
  case world::BaseObject::COLLIDER_TYPE::CIRCLE:
    return checkSecond<check>(*static_cast<const world::Circle &>(a).collider,
                              b);
  case world::BaseObject::COLLIDER_TYPE::POLYGON:
    return checkSecond<check>(*static_cast<const world::Polygon &>(a).collider,
                              b);
  }
  throw std::runtime_error{"INVALID OBJECT COLLISION QUERY"};
}
} // namespace collision
