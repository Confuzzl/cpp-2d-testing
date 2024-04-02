module scene;

import object;
import polygon;
import circle;

import debug;

import vector;
import bvh;
import aabb;
import math;

static std::vector<aabb_t> objects() {
  static constexpr unsigned count = 10;
  std::vector<aabb_t> out{};
  out.reserve(count);
  for (int i = 0; i < count; i++) {
    const glm::vec2 start{random_float(-2.f, +2.f), random_float(-2.f, +2.f)};
    const glm::vec2 size{random_float(0.5f, 1.f), random_float(0.5f, 1.f)};
    out.emplace_back(start, start + size);
    println(out[i]);
  }
  return out;
}

Scene::Scene() : objs{}, objs2{objects()}, tree{objs2} {}
Scene::~Scene() = default;

void Scene::init() {
  tree.topDown();
  tree.print();
}
