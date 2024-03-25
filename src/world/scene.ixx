export module scene;

import glm;
import rendering;
import camera;
import <set>;
import <memory>;

export struct base_obj_t;

export struct Scene {
  Camera camera{{0, 0}, 1};

  std::set<std::unique_ptr<base_obj_t>> objs{};

  Scene();
  ~Scene();

  void init();
};