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
  const auto [ent, b, p] = MAIN_SCENE.ecs.newEntity(
      ecs::Boundable{box}, ecs::Transformable{.position{pos}});
  MAIN_SCENE.data.insert(ent, b->localBounds + p->position);
  return ent;
}
static auto newPhys(const BoundingBox &box, const glm::vec2 pos,
                    const glm::vec2 vel, const glm::vec2 acc,
                    const float mass = 1) {
  const auto [ent, b, p, phy] = MAIN_SCENE.ecs.newEntity(
      ecs::Boundable{box}, ecs::Transformable{.position{pos}},
      ecs::Physical{.linear{.velocity{vel}, .acceleration{acc}, .mass{mass}}});
  MAIN_SCENE.data.insert(ent, b->localBounds + p->position);
  return ent;
}

static auto remove(size_t &ent,
                   const BoundingBox &box = MAIN_SCENE.data.BOUNDS) {
  const bool a = MAIN_SCENE.data.remove(ent, box);
  if (a)
    ECS.deleteEntity(ent);
}

Scene::Scene() : data{/*4, 8, 1 << 8*/ 2, 3, 4} {}

void Scene::init() {
  debugln(false, "SCENE INIT");
  // auto a = newBound({{0, 0}, {0.1, 0.1}}, {-0.2, 0.2});
  // auto b = newBound({{0, 0}, {0.1, 0.1}}, {1.3, -0.8});
  // auto c = newBound({{0, 0}, {0.1, 0.1}}, {-1.0, -0.8});
  // auto d = newBound({{0, 0}, {0.1, 0.1}}, {0.6, 0.3});
  // auto e = newBound({{0, 0}, {0.1, 0.1}}, {1.6, 1.3});
  // auto f = newBound({{0, 0}, {0.1, 0.1}}, {2.3, 0.7});
  // auto g = newBound({{0, 0}, {0.1, 0.1}}, {3.3, 2.9});

  // for (auto i = 0u; i < 50; i++) {
  //   const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
  //   const BoundingBox bounds{-size, +size};
  //   const auto pos = random_vec({-3, -3}, {+3, +3});
  //   newBound(bounds, pos);
  // }

  for (auto i = 0u; i < 10; i++) {
    const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
    newPhys({-size, +size},
            random_vec(data.BOUNDS.min + size, data.BOUNDS.max - size),
            random_vec({-2, -2}, {+2, +2}),
            /*random_vec({-1, -1}, {+1, +1})*/ {}, random_float(0.1f, 2.0f));
  }

  debugln(false, "SCENE COMPLETE\n"
                 "==================================");
}

void Scene::update(const double dt) {
  // debugln(false, "===NEW FRAME===");
  for (auto [id, trans, phys] :
       ecs.viewComponents<ecs::Transformable, ecs::Physical>()) {
    auto &[position, rotation] = *trans;
    auto &[lin, rot] = *phys;
    {
      auto &[vel, acc, mass] = lin;
      vel += acc * static_cast<float>(dt);
      position += vel * static_cast<float>(dt);
    }
    {
      auto &[vel, acc, mass] = rot;
      vel += acc * static_cast<float>(dt);
      rotation += vel * static_cast<float>(dt);
    }
  }
  // std::vector<std::pair<bool, bool>> pairs;

  data.cleanup();
}