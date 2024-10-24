module scene;

import debug;
import object;

import ecs_component;

void Scene::init() {
  for (auto i = 0; i < 5; i++)
    ecs.newEntity(ecs::Positionable{.position{i, i}});
}
