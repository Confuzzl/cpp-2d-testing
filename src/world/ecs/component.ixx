export module ecs_component;

import aabb;
import glm;
import mesh;

export namespace ecs {
inline namespace comp {
struct Positionable {
  glm::vec2 position;
};
struct Rotatable {
  float rotation;
};
struct LinearPhysical {
  glm::vec2 velocity;
  float acceleration;
  float mass;
};
struct RotationalPhysical {
  float angularVelocity;
  float angularAcceleration;
  float angularMass;
};
struct Polygonal {};
struct Circular {};
struct Boundable {
  BoundingBox bounds;
};
struct Renderable {
  Mesh mesh;
};
} // namespace comp
} // namespace ecs
