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

  // remove(d, {{0, 0}, {2, 2}});
  // remove(e, {{0, 0}, {2, 2}});
  // remove(f, {{0, 0}, {2, 2}});
  // remove(g, {{0, 0}, {2, 2}});

  // std::vector<std::pair<BoundingBox, glm::vec2>> boxes;
  // boxes.reserve(30);

  // for (auto i = 0u; i < 30; i++) {
  //   const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
  //   const BoundingBox bounds{-size, +size};
  //   const auto pos = random_vec({-3, -3}, {+3, +3});
  //   newBound(bounds, pos);
  //   boxes.emplace_back(bounds, pos);
  // }

  std::vector<std::pair<BoundingBox, glm::vec2>> boxes{
      {{{-0.21953356, -0.21695694}, {0.21953356, 0.21695694}},
       {0.5526717, 0.18073797}},
      {{{-0.18933713, -0.13644291}, {0.18933713, 0.13644291}},
       {-0.167593, -0.5120311}},
      {{{-0.16532205, -0.119659536}, {0.16532205, 0.119659536}},
       {0.5118637, 0.1732657}},
      {{{-0.19386527, -0.23102766}, {0.19386527, 0.23102766}},
       {-0.62781644, 0.82737875}},
      {{{-0.18537608, -0.1417554}, {0.18537608, 0.1417554}},
       {-0.8539376, 0.79287744}},
  };

  // std::vector<std::pair<BoundingBox, glm::vec2>> boxes{
  //     {{{-0.21953356, -0.21695694}, {0.21953356, 0.21695694}},
  //      {0.5526717, 0.18073797}}, // <-- problem
  //     {{{-0.19437581, -0.15245189}, {0.19437581, 0.15245189}},
  //      {0.552454, -2.6598992}},
  //     {{{-0.11138651, -0.16839465}, {0.11138651, 0.16839465}},
  //      {-1.4091563, 0.25767136}},
  //     {{{-0.113859795, -0.15897267}, {0.113859795, 0.15897267}},
  //      {2.8717155, 1.293715}},
  //     {{{-0.13919157, -0.199617}, {0.13919157, 0.199617}},
  //      {-1.0485653, 2.3553505}},
  //     {{{-0.2044869, -0.10950263}, {0.2044869, 0.10950263}},
  //      {-1.7870928, 1.455121}},
  //     {{{-0.14432028, -0.20984423}, {0.14432028, 0.20984423}},
  //      {1.2882891, -2.1663704}},
  //     {{{-0.14435592, -0.12391786}, {0.14435592, 0.12391786}},
  //      {0.2855053, 2.2127304}},
  //     {{{-0.18485391, -0.11137713}, {0.18485391, 0.11137713}},
  //      {-0.12996864, 2.9475918}},
  //     {{{-0.18933713, -0.13644291}, {0.18933713, 0.13644291}},
  //      {-0.167593, -0.5120311}},
  //     {{{-0.16415587, -0.13672037}, {0.16415587, 0.13672037}},
  //      {-1.7523717, 0.47740602}},
  //     {{{-0.16611472, -0.12116563}, {0.16611472, 0.12116563}},
  //      {0.19475889, -1.3180879}},
  //     {{{-0.11016396, -0.12655538}, {0.11016396, 0.12655538}},
  //      {-2.3429823, -0.57191205}},
  //     {{{-0.16532205, -0.119659536}, {0.16532205, 0.119659536}},
  //      {0.5118637, 0.1732657}},
  //     {{{-0.19386527, -0.23102766}, {0.19386527, 0.23102766}},
  //      {-0.62781644, 0.82737875}},
  //     {{{-0.112948686, -0.14661352}, {0.112948686, 0.14661352}},
  //      {0.046227932, -1.2129629}},
  //     {{{-0.16498661, -0.1482352}, {0.16498661, 0.1482352}},
  //      {-2.064261, 2.9379787}},
  //     {{{-0.107021295, -0.20972633}, {0.107021295, 0.20972633}},
  //      {-2.6909976, -1.8757168}},
  //     {{{-0.22718805, -0.13405778}, {0.22718805, 0.13405778}},
  //      {-2.7633052, 0.79860544}},
  //     {{{-0.22360884, -0.2339772}, {0.22360884, 0.2339772}},
  //      {-1.6612797, 0.7753458}},
  //     {{{-0.23508883, -0.14325666}, {0.23508883, 0.14325666}},
  //      {-0.41326642, -1.7055418}},
  //     {{{-0.119655736, -0.18073669}, {0.119655736, 0.18073669}},
  //      {1.618701, 2.094152}},
  //     {{{-0.121741965, -0.15655625}, {0.121741965, 0.15655625}},
  //      {-0.6409054, -1.5692188}},
  //     {{{-0.11731489, -0.220405}, {0.11731489, 0.220405}},
  //      {2.1782727, -2.3202958}},
  //     {{{-0.18537608, -0.1417554}, {0.18537608, 0.1417554}},
  //      {-0.8539376, 0.79287744}},
  //     {{{-0.10646281, -0.23151988}, {0.10646281, 0.23151988}},
  //      {-2.8800168, 1.3852692}},
  //     {{{-0.106167905, -0.10238099}, {0.106167905, 0.10238099}},
  //      {2.157175, -1.7727942}},
  //     {{{-0.122532375, -0.2369996}, {0.122532375, 0.2369996}},
  //      {-1.6787795, -0.9876952}},
  //     {{{-0.19907147, -0.21353212}, {0.19907147, 0.21353212}},
  //      {1.6888995, 2.346424}},
  //     {{{-0.23480806, -0.24475813}, {0.23480806, 0.24475813}},
  //      {1.1850905, 1.6826735}},
  // };

  // std::string gen = "std::vector<std::pair<BoundingBox, glm::vec2>>
  // boxes{\n"; for (const auto [box, pos] : boxes) {
  //   if (!BoundingBox{{-1, -1}, {1, 1}}.contains(pos))
  //     continue;

  //  gen +=
  //      std::format("{{{{{{{}, {}}}, {{{}, {}}}}}, {{{}, {}}}}},\n",
  //      box.min.x,
  //                  box.min.y, box.max.x, box.max.y, pos.x, pos.y);
  //}
  // gen += "};";
  // println(gen);

  for (const auto [box, pos] : boxes) {
    newBound(box, pos);
  }

  // debugln(true, "nodes:");
  // for (auto i = 0u; i < data.nodes.size(); i++) {
  //   const auto &node = data.nodes[i];
  //   debugln(true, "{}: first={} count={}", i, node.first, node.count);
  // }
  // debugln(true, "element nodes:");
  // for (auto i = 0u; i < data.elementNodeCount; i++) {
  //   const auto &node = data.elementNodes[i];
  //   debugln(true, "{}: next={} index={}", i, node.next, node.elementIndex);
  // }
  // debugln(true, "elements:");
  // for (auto i = 0u; i < data.elementCount; i++) {
  //   const auto &node = data.elements[i];
  //   debugln(true, "{}: id={}", i, node.ent);
  // }

  // static constexpr BoundingBox queryBox{{-1, -1}, {1, 1}};
  //  const auto [list, min, max] = data.queryLeaves(queryBox);
  //  for (const auto elementIndex : list) {
  //    const auto [id, box] = data.elements[elementIndex];
  //    println("{}: {}", id, box);
  //  }
  //  println("============");
  //  const auto query = data.queryAll(queryBox);
  //  println(query.size());
  //  for (const auto [id, box] : query) {
  //   println("{}: {}", id, box);
  // }

  // for (auto i = 0u; i < 10; i++) {
  //   const auto size = random_vec({0.1, 0.1}, {0.25, 0.25});
  //   newPhys({-size, +size}, random_vec({-3, -3}, {+3, +3}),
  //           random_vec({-2, -2}, {+2, +2}), random_vec({-1, -1}, {+1,
  //           +1}));
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