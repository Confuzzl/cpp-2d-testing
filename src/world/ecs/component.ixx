export module ecs_component;

import aabb;
import glm;
import mesh;
import <functional>;
import runtime_array;

export struct BaseFrame;

export namespace ecs {
inline namespace comp {
struct Transformable {
  glm::vec2 position;
  float rotation = 0;
};
struct Physical {
  struct {
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass = 0;
  } linear;
  struct {
    float velocity;
    float acceleration;
    float mass;
  } angular;
};
struct Boundable {
  BoundingBox localBounds;
};
struct Renderable {
  Mesh mesh;
};
struct DirectRenderable {
  std::function<void(BaseFrame *)> draw;
};
} // namespace comp
} // namespace ecs
