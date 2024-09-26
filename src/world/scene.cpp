module scene;

import debug;
import object;

void Scene::init() {
  std::vector<world::BaseObject *> a{};
  // a.emplace_back(world::Circle{})
  tree = collision::BoundingVolumeHierarchy::from(a);
  // tree.topDown();
  //  tree.print();
}
