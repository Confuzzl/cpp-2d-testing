module scene;

import debug;
import object;

void Scene::init() {

  std::vector<world::BaseObject *> a{};
  tree = collision::BoundingVolumeHierarchy::from(a);
  // tree.topDown();
  //  tree.print();
}
