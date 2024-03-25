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
  std::vector<aabb_t> objs = vec::New<aabb_t>();

  // bvh::tree_t tree{};
  // tree.print();
}