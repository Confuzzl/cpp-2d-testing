export module ecs_component;

import aabb;
import glm;
import mesh;
import <functional>;
import runtime_array;
import shapes;

export struct BaseFrame;

export namespace ecs {
inline namespace comp {
struct Transformable {
  glm::vec2 position;
  float rotation = 0;
};
struct Physical {
  struct Linear {
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass = 0;
  } linear;
  struct Angular {
    float velocity = 0;
    float acceleration = 0;
    float mass = 0;
  } angular;
};
struct Boundable {
  BoundingBox localBounds;
};
struct Collidable {
  collision::Collider collider;
};

struct Renderable {
  Mesh mesh;
};
struct DirectRenderable {
  std::function<void(BaseFrame *)> draw;
};
} // namespace comp
} // namespace ecs
