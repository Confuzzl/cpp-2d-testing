export module scene;

import camera;
import <map>;
import <memory>;
// import bvh2;
import ecs_manager;

export struct Scene {
  Camera camera{{0, 0}};

  ecs::Manager ecs;

  // collision::BoundingVolumeHierarchy tree;

  // Scene();
  //~Scene();

  void init();
  void update(const double dt);
};