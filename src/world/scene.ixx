export module scene;

import glm;
import rendering;
import camera;
import <set>;
import <memory>;
import <vector>;

import bvh;

// import polygon;
// import circle;
import color;

export struct base_obj_t;
export struct BoundingBox;

export struct Scene {
  Camera camera{{0, 0}};

  // std::set<std::unique_ptr<base_obj_t>> objs;

  // std::vector<std::unique_ptr<base_obj_t>> objects;
  // void addPolygon(const Polygon::opts_t &poly_opts,
  //                 const glm::vec2 pos = {0, 0}, const float rot = 0,
  //                 const Color color = colors::random());
  // void addQuadToFrom(const Polygon::opts_t &poly_opts, const glm::vec2 from,
  //                    const glm::vec2 to);
  // void addQuadSize(const Polygon::opts_t &opts, const glm::vec2 from,
  //                  const glm::vec2 to);
  // void addCircle(const float radius = 1, const glm::vec2 &pos = {0, 0},
  //                const float rot = 0, const Color color =
  //                colors::random());

  // std::vector<aabb_t> objs2;
  // bvh::tree_t tree;

  // Scene();
  //~Scene();

  void init();
};