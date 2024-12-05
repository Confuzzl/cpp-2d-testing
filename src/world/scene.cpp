module;

#include "util/main_objects.h"

module scene;

import debug;
import ecs_component;

import app;
import <functional>;
import frame;
import aabb;
import glm;
import hash_grid;

// import math;

namespace ecs {
template <> void onAdd(const EntID ent, Boundable &comp) {
  MAIN_SCENE.grid.insert(ent, comp.localBounds);
}
template <> void onRemove(const EntID ent, Boundable &comp) {
  MAIN_SCENE.grid.remove(ent, comp.localBounds);
}
} // namespace ecs

void Scene::init() {
  // ecs.newEntity(ecs::Boundable{.bounds{{-1, -1}, {1, 1}}});

  // for (auto i = 0u; i < 100; i++) {
  //   const auto start = random_vec({-5, -5}, {3, 3});
  //   const auto size = random_vec({0, 0}, {2, 2});
  //   ecs.newEntity(ecs::Boundable{.bounds{start, start + size}});
  // }

  // auto [id, pos, linPhys, rend] =
  //     ecs.newEntity(ecs::Positionable{.position{1, 0}},
  //                   ecs::LinearPhysical{.velocity{0, 1}, .acceleration{0,
  //                   0}}, ecs::DirectRenderable{});
  // rend->func = [&pos = pos->position](BaseFrame *frame) {
  //   frame->drawCircle(pos, 0.25f, colors::RED);
  // };
}

void Scene::update(const double dt) {
  for (const auto [id, pos, bound] :
       ecs.viewComponents<ecs::Positionable, ecs::Boundable>()) {
    bound->position = pos->position;
  }
  for (const auto [id, pos, linPhys] :
       ecs.viewComponents<ecs::Positionable, ecs::LinearPhysical>()) {
    auto &[position] = *pos;
    auto &[velocity, acceleration, mass] = *linPhys;

    // symplectic euler
    velocity += acceleration * static_cast<float>(dt);
    position += velocity * static_cast<float>(dt);
  }
}