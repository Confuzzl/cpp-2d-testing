export module ecs_component;

import aabb;
import glm;
import mesh;
import <functional>;
import runtime_array;

export struct BaseFrame;

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
  glm::vec2 acceleration;
  float mass;
};
struct RotationalPhysical {
  float angularVelocity;
  float angularAcceleration;
  float angularMass;
};
struct Polygonal {
  runtime_array<glm::vec2> vertices;
};
struct Circular {
  float radius;
};
struct Boundable {
  BoundingBox bounds;
};
struct Renderable {
  Mesh mesh;
};
struct DirectRenderable {
  std::function<void(BaseFrame *)> draw;
};
} // namespace comp
} // namespace ecs
