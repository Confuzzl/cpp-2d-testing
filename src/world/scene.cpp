module;

#include "util/main_objects.h"

module scene;

import debug;
import ecs_component;

import app;
import frame;
import aabb;
import glm;
import hash_grid;

// import math;

// namespace ecs {
// template <> void onAdd(const EntID ent, Boundable &comp) {
//   MAIN_SCENE.data.insert(ent, comp.localBounds);
// }
// template <> void onRemove(const EntID ent, Boundable &comp) {
//   MAIN_SCENE.data.remove(ent, comp.localBounds);
// }
// } // namespace ecs

static auto newBound(const BoundingBox &box, const glm::vec2 pos) {
  const auto [ent, b, p] =
      MAIN_SCENE.ecs.newEntity(ecs::Boundable{box}, ecs::Positionable{pos});
  MAIN_SCENE.data.insert(ent, b->localBounds + p->position);
  return ent;
}

static auto remove(size_t &ent,
                   const BoundingBox &box = collision::Quadtree::BOUNDS) {
  println("removing {}", ent);
  const bool a = MAIN_SCENE.data.remove(ent, box);
  if (a)
    MAIN_SCENE.ecs.deleteEntity(ent);
}

void Scene::init() {
  auto a = newBound({{-0.1, -0.1}, {+0.1, +0.1}}, {-0.3, -0.3});
  auto b = newBound({{-0.1, -0.1}, {+0.1, +0.1}}, {0.3, 0.3});
  auto c = newBound({{-0.1, -0.1}, {+0.1, +0.1}}, {1.3, 1.3});
  auto d = newBound({{-0.1, -0.1}, {+0.1, +0.1}}, {2.3, 2.2});

  println("=====================================");

  remove(a);
  remove(b, {{0, 0}, {1, 1}});
  remove(c, {{0, 0}, {1, 1}});
  remove(d);

  println("=====================================");

  newBound({{-0.5, -0.5}, {+0.5, +0.5}}, {1.3, 1.3});

  //  data.remove(1, collision::Quadtree::BOUNDS);
  //  data.remove(2, collision::Quadtree::BOUNDS);
  //  data.remove(3, collision::Quadtree::BOUNDS);

  // for (auto i = 0u; i < 100; i++) {
  //   const auto pos = random_vec({-4, -4}, {4, 4});
  //   const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
  //   println(BoundingBox{-size, +size} + pos);
  //   newBound({-size, +size}, pos);
  // }
  println("=====================================");
  for (const auto &node : data.nodes) {
    println("{} first: {} count: {}", node.isLeaf(), node.first, node.count);
  }
  // for (auto i : {0, 1, 3}) {
  //   println("{}: index: {} next: {} id: {}", i,
  //           data.elementNodes[i].elementIndex, data.elementNodes[i].next,
  //           data.elements[data.elementNodes[i].elementIndex].ent);
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
    auto &[position] = *pos;
    auto &[velocity, acceleration, mass] = *linPhys;

    // symplectic euler
    velocity += acceleration * static_cast<float>(dt);
    position += velocity * static_cast<float>(dt);
  }
  for (const auto [id, pos, bounds] :
       ecs.viewComponents<ecs::Positionable, ecs::Boundable>()) {
    const BoundingBox box = bounds->localBounds + pos->position;
  }

  data.cleanup();
}