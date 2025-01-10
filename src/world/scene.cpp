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

static auto newBound(const BoundingBox &box, const glm::vec2 pos) {
  const auto [ent, b, p] =
      MAIN_SCENE.ecs.newEntity(ecs::Boundable{box}, ecs::Positionable{pos});
  MAIN_SCENE.data.insert(ent, b->localBounds + p->position);
  return ent;
}
static auto newPhys(const BoundingBox &box, const glm::vec2 pos,
                    const glm::vec2 vel, const glm::vec2 acc,
                    const float mass = 1) {
  const auto [ent, b, p, phy] = MAIN_SCENE.ecs.newEntity(
      ecs::Boundable{box}, ecs::Positionable{pos},
      ecs::LinearPhysical{.velocity{vel}, .acceleration{acc}, .mass{mass}});
  MAIN_SCENE.data.insert(ent, b->localBounds + p->position);
  return ent;
}

static auto remove(size_t &ent,
                   const BoundingBox &box = MAIN_SCENE.data.BOUNDS) {
  const bool a = MAIN_SCENE.data.remove(ent, box);
  if (a)
    ECS.deleteEntity(ent);
}

Scene::Scene() : data{/*2, 8, 1 << 8*/ 2, 3, 4} {}

void Scene::init() {
  debugln(false, "SCENE INIT");
  // auto a = newBound({{0, 0}, {0.1, 0.1}}, {-0.2, 0.2});
  // auto b = newBound({{0, 0}, {0.1, 0.1}}, {1.3, -0.8});
  // auto c = newBound({{0, 0}, {0.1, 0.1}}, {-1.0, -0.8});
  // auto d = newBound({{0, 0}, {0.1, 0.1}}, {0.6, 0.3});
  // auto e = newBound({{0, 0}, {0.1, 0.1}}, {1.6, 1.3});
  // auto f = newBound({{0, 0}, {0.1, 0.1}}, {2.3, 0.7});
  // auto g = newBound({{0, 0}, {0.1, 0.1}}, {3.3, 2.9});

  // remove(d);
  // remove(e);
  // remove(f);
  // remove(g);

  // data.cleanup();

  // debugln(false, "=====================");

  // const glm::vec2 size{0.15f};
  // const glm::vec2 pos{0, 0};
  //// newBound({-size, +size}, pos);
  // newPhys({-size, +size}, pos, {1, 1}, {});

  // debugln(false, "=====================");
  // debugln(false, "nodes:");
  // for (auto i = 0u; i < data.nodes.size(); i++) {
  //   const auto &node = data.nodes[i];
  //   debugln(false, "{}: first={} count={}", i, node.first, node.count);
  // }
  // debugln(false, "element nodes:");
  // for (auto i = 0u; i < data.elementNodeCount; i++) {
  //   const auto &node = data.elementNodes[i];
  //   debugln(false, "{}: next={} index={}", i, node.next, node.elementIndex);
  // }
  // debugln(false, "elements:");
  // for (auto i = 0u; i < data.elementCount; i++) {
  //   const auto &node = data.elements[i];
  //   debugln(false, "{}: id={}", i, node.ent);
  // }

  // BoundingBox box{{-0.3, -0.7}, {0.3, 0.7}};
  // glm::vec2 oldPos{2.3, -1.3};
  // glm::vec2 newPos{2.9, -0.3};
  // auto foo = newBound(box, oldPos);
  // remove(foo, box + oldPos);
  // newBound(box, newPos);

  // for (const auto &node : data.nodes) {
  //   println("first={} count={}", node.first, node.count);
  // }

  // remove(d, {{0, 0}, {2, 2}});
  // remove(e, {{0, 0}, {2, 2}});
  // remove(f, {{0, 0}, {2, 2}});
  // remove(g, {{0, 0}, {2, 2}});

  // data.cleanup();

  // newBound({{0, 0}, {0.1, 0.1}}, {2.1, 0.3});

  // newPhys({{-0.1, -0.1}, {0.1, 0.1}}, {1, 0}, {0, 1}, {});

  // auto eee = newBound({{-0.1, -0.1}, {0.1, 0.1}}, {0, 0});
  // println("========================");
  // remove(eee, {{-0.5, -0.5}, {0.5, 0.5}});

  for (auto i = 0u; i < 20; i++) {
    const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
    newBound({-size, +size}, random_vec({-3, -3}, {+3, +3}));
  }

  data.queryAll({{0, 0}, {1, 1}});

  // for (auto i = 0u; i < 10; i++) {
  //   const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
  //   newPhys({-size, +size}, random_vec({-3, -3}, {+3, +3}),
  //           random_vec({-2, -2}, {+2, +2}), random_vec({-1, -1}, {+1, +1}));
  // }

  debugln(false, "SCENE COMPLETE\n"
                 "==================================");
}

void Scene::update(const double dt) {
  // debugln(false, "===NEW FRAME===");

  for (const auto [id, bound, pos, linPhys] :
       ecs.viewComponents<ecs::Boundable, ecs::Positionable,
                          ecs::LinearPhysical>()) {
    auto [localBounds] = *bound;
    auto &[position] = *pos;
    auto &[velocity, acceleration, mass] = *linPhys;

    const auto oldBox = localBounds + position;

    // symplectic euler
    velocity += acceleration * static_cast<float>(dt);
    position += velocity * static_cast<float>(dt);

    const auto newBox = localBounds + position;

    if (!data.BOUNDS.intersects(newBox)) {
      // velocity = -velocity;
      //   acceleration = -acceleration;
      //     position = {1, 0};
      position = random_vec({-1, -1}, {+1, 1});
      velocity = random_vec({-2, -2}, {+2, +2});
      acceleration = random_vec({-1, -1}, {+1, +1});
    }

    if (/*velocity != glm::vec2{0}*/ true) {
      data.remove(id, oldBox);
      data.insert(id, newBox);
    }
  }

  data.cleanup();
}