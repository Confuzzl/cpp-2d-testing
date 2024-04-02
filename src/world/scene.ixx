export module scene;

import glm;
import rendering;
import camera;
import <set>;
import <memory>;
import <vector>;

import bvh;

export struct base_obj_t;
export struct aabb_t;

export struct Scene {
  Camera camera{{0, 0}, 1};

  std::set<std::unique_ptr<base_obj_t>> objs;

  std::vector<aabb_t> objs2;
  bvh::tree_t tree;

  Scene();
  ~Scene();

  void init();
};