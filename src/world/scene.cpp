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
  MAIN_SCENE.grid.insert(ent, comp.bounds);
}
template <> void onRemove(const EntID ent, Boundable &comp) {
  MAIN_SCENE.grid.remove(ent, comp.bounds);
}
} // namespace ecs

void Scene::init() {
  ecs.newEntity(ecs::Boundable{.bounds{{-1, -1}, {1, 1}}});

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
  for (const auto [id, pos, linPhys] :
       ecs.viewComponents<ecs::Positionable, ecs::LinearPhysical>()) {
    if (id != 0)
      continue;
    auto &[position] = *pos;
    auto &[velocity, acceleration, mass] = *linPhys;

    // ecs.newEntity(ecs::DirectRenderable{
    //     .func{[dt, position](BaseFrame *frame) {
    //       frame->drawPointFixed(
    //           position, 10,
    //           colors::random_i(*reinterpret_cast<const unsigned int
    //           *>(&dt)));
    //     }}});

    // symplectic euler
    velocity += acceleration * static_cast<float>(dt);
    position += velocity * static_cast<float>(dt);
  }
}