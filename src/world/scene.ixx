export module scene;

import camera;
import <map>;
import <memory>;
// import bvh2;
import ecs_manager;

import hash_grid;
import quadtree;

// export namespace ecs {
// inline namespace comp {
// struct Boundable;
// }
// template <> void onAdd(const EntID ent, Boundable &comp);
// template <> void onRemove(const EntID ent, Boundable &comp);
// } // namespace ecs

export struct Scene {
  Camera camera{{0, 0}};

  ecs::Manager ecs;

  // collision::BoundingVolumeHierarchy tree;

  using Accel = collision::Quadtree;

  // collision::SpatialHashGrid grid;
  Accel data;

  // Scene();
  //~Scene();

  void init();
  void update(const double dt);
};