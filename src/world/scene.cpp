module scene;

import debug;

import ecs_component;

import <functional>;
import frame;
import dimensions;
import glm;

void Scene::init() {
  auto [id, pos, linPhys, rend] =
      ecs.newEntity(ecs::Positionable{.position{1, 0}},
                    ecs::LinearPhysical{.velocity{0, 1}, .acceleration{0, 0}},
                    ecs::DirectRenderable{});
  rend->func = [&pos = pos->position](BaseFrame *frame) {
    frame->drawCircle(pos, 0.25f, colors::RED);
  };
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

    acceleration = -position;
    velocity += acceleration * static_cast<float>(dt);
    position += velocity * static_cast<float>(dt);
  }
}