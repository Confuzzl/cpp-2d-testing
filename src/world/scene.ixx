export module scene;

import camera;
import <map>;
import <memory>;
// import bvh2;
import ecs_manager;

// export struct BoundingBox;

export struct Scene {
  Camera camera{{0, 0}};

  ecs::Manager ecs;

  // std::map<unsigned int, std::unique_ptr<world::BaseObject>> objectList;

  // collision::BoundingVolumeHierarchy tree;

  // Scene();
  //~Scene();

  void init();
};