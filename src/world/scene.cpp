module scene;

import object;
import polygon;
import circle;

Scene::Scene() {}
Scene::~Scene() = default;

import debug;

import vector;
import bvh;
import aabb;

void Scene::init() {
  std::vector<aabb_t> objs{4, {{0, 0}, {1, 1}}};

  bvh::tree_t tree{objs};
  tree.print();
}