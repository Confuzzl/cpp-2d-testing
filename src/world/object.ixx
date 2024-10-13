module;

#include "util/gl.h"

export module object;

import glm;
import collision;
import mesh;
import <memory>;

// struct TypeID {
//   unsigned int data;
//   bool type() const { return data >> 32; }
//   unsigned int ID() const { return data & 0x7f'ff'ff'ff; }
// };
//
// export namespace world {
// struct BaseObject {
//  enum COLLIDER_TYPE : bool { CIRCLE, POLYGON };
//
// protected:
//  std::unique_ptr<collision::Collider> collider;
//  COLLIDER_TYPE type;
//
//  float mass;
//  glm::vec2 velocity, acceleration;
//
//  float angMass;
//  float angVelocity, angAcceleration;
//
//  // float friction = 0;
//  unsigned int ID;
//
//  std::unique_ptr<Mesh> mesh;
//
// public:
//  BaseObject(std::unique_ptr<collision::Collider> &&collider,
//             const COLLIDER_TYPE type, Mesh &&mesh)
//      : collider{std::move(collider)}, type{type},
//        mesh{std::make_unique<Mesh>(std::move(mesh))} {}
//
//  void remove() {}
//
//  virtual collision::Collider *getCollider() { return collider.get(); }
//  virtual const collision::Collider *getCollider() const {
//    return collider.get();
//  }
//  virtual collision::Collider *operator->() { return getCollider(); }
//  virtual const collision::Collider *operator->() const {
//    return getCollider();
//  }
//
//  COLLIDER_TYPE getType() const { return type; }
//  Mesh &getMesh() { return *mesh; }
//  const Mesh &getMesh() const { return *mesh; }
//};
// template <typename T> struct Object : BaseObject {
//  Object(std::unique_ptr<T> &&collider, const COLLIDER_TYPE type, Mesh &&mesh)
//      : BaseObject(std::move(collider), type, std::move(mesh)) {}
//
//  T *getCollider() override { return static_cast<T *>(collider.get()); }
//  const T *getCollider() const override {
//    return static_cast<const T *>(collider.get());
//  }
//  T *operator->() override { return getCollider(); }
//  const T *operator->() const override { return getCollider(); }
//};
// struct Circle : Object<collision::Circle> {
//  Circle(std::unique_ptr<collision::Circle> &&collider, Mesh &&mesh)
//      : Object(std::move(collider), CIRCLE, std::move(mesh)) {}
//};
// struct Polygon : Object<collision::Polygon> {
//  Polygon(std::unique_ptr<collision::Polygon> &&collider, Mesh &&mesh)
//      : Object(std::move(collider), POLYGON, std::move(mesh)) {}
//};
//
//} // namespace world
//
// namespace collision {
// template <bool check = true, typename A>
// bool checkSecond(const A &a, const world::BaseObject &b) {
//  switch (b.getType()) {
//  case world::BaseObject::COLLIDER_TYPE::CIRCLE:
//    return query<check>(a,
//                        *static_cast<const world::Circle &>(b).getCollider());
//  case world::BaseObject::COLLIDER_TYPE::POLYGON:
//    return query<check>(a,
//                        *static_cast<const world::Polygon
//                        &>(b).getCollider());
//  }
//  throw std::runtime_error{"INVALID OBJECT COLLISION QUERY"};
//}
//} // namespace collision
// export namespace collision {
// template <bool check = true>
// bool query(const world::BaseObject &a, const world::BaseObject &b) {
//  switch (a.getType()) {
//  case world::BaseObject::COLLIDER_TYPE::CIRCLE:
//    return checkSecond<check>(
//        *static_cast<const world::Circle &>(a).getCollider(), b);
//  case world::BaseObject::COLLIDER_TYPE::POLYGON:
//    return checkSecond<check>(
//        *static_cast<const world::Polygon &>(a).getCollider(), b);
//  }
//  throw std::runtime_error{"INVALID OBJECT COLLISION QUERY"};
//}
//} // namespace collision
