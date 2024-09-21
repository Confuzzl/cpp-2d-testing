module;

#include <ranges>

module bvh2;

using namespace collision;

import <algorithm>;

static BoundingBox toBox(const BoundingVolumeHierarchy::T *const o) {
  return o->getCollider()->getAABB();
}
static float med(const BoundingVolumeHierarchy::T *const o, const bool axis) {
  return toBox(o).median()[axis];
}

BoundingVolumeHierarchy
BoundingVolumeHierarchy::topDown(const std::vector<T *> &list) {
  auto arr = make_runtime_array<T *>(list);
  auto root = std::make_unique<Node>();
  topDownRecurse(root.get(), {arr}, 0);
  return {std::move(root)};
}
void BoundingVolumeHierarchy::topDownRecurse(Node *const node,
                                             std::span<T *> objects,
                                             const int depth) {
  for (const T *const o : objects)
    node->box.expand(toBox(o));

  if (objects.size() > MAX_OBJECTS_PER_LEAF) {
    const bool axis = node->box.width() < node->box.height();
    const float median = node->box.median()[axis];
    auto split = std::partition(
        objects.begin(), objects.end(),
        [axis, median](const T *const o) { return med(o, axis) <= median; });
    // partition unable to create 2 groups
    // sort and then split in half
    if (split - objects.begin() == 0 || objects.end() - split == 0) {
      std::sort(objects.begin(), objects.end(),
                [axis](const T *const a, const T *const b) {
                  return med(a, axis) < med(b, axis);
                });
      split = objects.begin() + objects.size() / 2;
    }

    node->setChildren(std::make_unique<Node>(), std::make_unique<Node>());
    auto left = node->getChildren().left.get(),
         right = node->getChildren().right.get();

    topDownRecurse(left, {objects.begin(), split}, depth + 1);
    topDownRecurse(right, {split, objects.end()}, depth + 1);
  } else {
    node->setArray({objects});
  }
}