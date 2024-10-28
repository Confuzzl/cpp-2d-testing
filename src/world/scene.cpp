module scene;

import debug;
// import object;

import ecs_component;

import <functional>;
import frame;
import dimensions;

void Scene::init() {
  ecs.newEntity(ecs::DirectRenderable{
      .func{std::bind(&BaseFrame::drawBoxFixed, std::placeholders::_1,
                      Dimension{{0, 0}, {1, 1}}, 5.0f, colors::RED)}});
  // ecs.newEntity(ecs::FooRenderable{.func{}});
  //  for (auto i = 0; i < 5; i++)
  //    ecs.newEntity(ecs::Positionable{.position{i, i}});
}
