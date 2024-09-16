module;

#include <ranges>

module bvh2;

using namespace collision;

import <algorithm>;

static BoundingBox toBox(const BoundingVolumeHierarchy::T *const o) {
  return o->getCollider()->getAABB();
}

BoundingVolumeHierarchy
BoundingVolumeHierarchy::topDown(const std::vector<T *> &list) {
  auto arr = make_runtime_array<T *>(list);
  auto root = std::make_unique<Node>();
  topDownRecurse(root.get(), {arr});
  return {std::move(root)};
}
void BoundingVolumeHierarchy::topDownRecurse(Node *const node,
                                             std::span<T *> objects) {
  static constexpr auto partition = [](const std::span<BoundingBox> sorted,
                                       const bool axis, const float m) {
    auto it = sorted.begin();
    while (it != sorted.end()) {
      if (m < it->median()[axis])
        return it;
      it++;
    }
    return it;
  };

  for (const T *const o : objects)
    node->box.expand(toBox(o));

  if (objects.size() > MAX_OBJECTS_PER_LEAF) {
    const bool axis = node->box.width() < node->box.height();
    const float median = node->box.median()[axis];
    auto split = std::partition(objects.begin(), objects.end(),
                                [axis, median](const T *const o) {
                                  return toBox(o).median()[axis] < median;
                                });

    if (split - objects.begin() == 0 || objects.end() - split == 0)
      return;

    node->setChildren(std::make_unique<Node>(), std::make_unique<Node>());
    auto left = node->getChildren().left.get(),
         right = node->getChildren().right.get();

    topDownRecurse(left, {objects.begin(), split});
    topDownRecurse(right, {split, objects.end()});
  } else {
    node->setArray({objects});
  }
}