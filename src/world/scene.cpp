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
import shapes;
import <algorithm>;

static auto remove(size_t &ent,
                   const BoundingBox &box = MAIN_SCENE.data.BOUNDS) {
  const bool a = MAIN_SCENE.data.remove(ent, box);
  if (a)
    ECS.deleteEntity(ent);
}

// static template <typename T>
static auto newCircle(ecs::Transformable &&trans, ecs::Physical &&phys,
                      const float radius) {
  BoundingBox box{radius};
  using namespace collision;
  const auto [id, t, p, c, b] =
      MAIN_SCENE.ecs.newEntity(std::move(trans), std::move(phys),
                               ecs::Collidable{
                                   .collider{.data{
                                       Circle{.radius = radius},
                                   }},
                               },
                               ecs::Boundable{box});
  MAIN_SCENE.data.insert(id, box + t->position);
  return id;
}

Scene::Scene() : data{/*4, 8, 1 << 8*/ 2, 3, 4} {}

void Scene::init() {
  debugln(false, "SCENE INIT");

  using namespace collision;

  auto a = newCircle(
      ecs::Transformable{
          .position{-0.5, 0},
          .rotation = 0,
      },
      ecs::Physical{
          .linear{
              .velocity{0.5, 0.5},
          },
          .angular{},
      },
      0.5);
  auto b = newCircle(
      ecs::Transformable{
          .position{0.5, 0},
          .rotation = 0,
      },
      ecs::Physical{
          .linear{
              .velocity{-0.5, 0.5},
          },
          .angular{},
      },
      0.5);

  // for (auto i = 0u; i < 10; i++) {
  //   const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
  //   newPhys({-size, +size},
  //           random_vec(data.BOUNDS.min + size, data.BOUNDS.max - size),
  //           random_vec({-2, -2}, {+2, +2}),
  //           /*random_vec({-1, -1}, {+1, +1})*/ {},
  //           random_float(0.1f, 2.0f));
  // }

  debugln(false, "SCENE COMPLETE\n"
                 "==================================");
}

void Scene::update(const double dt) {
  // debugln(false, "===NEW FRAME===");
  using namespace ecs;
  for (auto [id, trans, phys] : ecs.viewComponents<Transformable, Physical>()) {
    auto &[pos, rot] = *trans;
    auto &[lin, ang] = *phys;
    {
      auto &[vel, acc, mass] = lin;
      vel += acc * static_cast<float>(dt);
      pos += vel * static_cast<float>(dt);
    }
    {
      auto &[vel, acc, mass] = ang;
      vel += acc * static_cast<float>(dt);
      rot += vel * static_cast<float>(dt);
    }
  }
  std::vector<std::pair<std::size_t, std::size_t>> pairs;
  for (auto [id, trans, bounds] :
       ecs.viewComponents<Transformable, Boundable>()) {
    auto [pos, rot] = *trans;
    const auto box = bounds->localBounds + pos;
    for (const auto [other, box] : data.queryAll(box, id)) {
      if (id < other)
        pairs.emplace_back(id, other);
      else
        pairs.emplace_back(other, id);
    }
  }
  std::ranges::sort(pairs, [](const std::pair<std::size_t, std::size_t> &a,
                              const std::pair<std::size_t, std::size_t> &b) {
    if (a.first == b.first)
      return a.second < b.second;
    return a.first < b.first;
  });
  const auto remove = std::ranges::unique(pairs);
  pairs.erase(remove.begin(), remove.end());

  for (const auto [a, b] : pairs) {
    auto [at, ap, ac] =
        ecs.getComponents<Transformable, Physical, Collidable>(a);
    auto [bt, bp, bc] =
        ecs.getComponents<Transformable, Physical, Collidable>(b);
    using namespace collision;
    ac->collider.visit(
        [bc](const Wall &wall) {
          bc->collider.visit([](const Wall &wall) {},
                             [](const Circle &circle) {},
                             [](const Polygon &polygon) {});
        },
        [bc](const Circle &circle) {
          bc->collider.visit([](const Wall &wall) {},
                             [](const Circle &circle) {},
                             [](const Polygon &polygon) {});
        },
        [bc](const Polygon &polygon) {
          bc->collider.visit([](const Wall &wall) {},
                             [](const Circle &circle) {},
                             [](const Polygon &polygon) {});
        });
  }

  // println("==========");
  // for (const auto [a, b] : pairs)
  //   println("{}-{}", a, b);

  data.cleanup();
}