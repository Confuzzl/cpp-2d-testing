module;

#include <ranges>

module bvh2;

using namespace collision;

import <algorithm>;
import runtime_array;

BoundingVolumeHierarchy
BoundingVolumeHierarchy::topDown(const std::vector<T *> &list) {
  static constexpr auto toBox = [](const T *o) {
    return o->getCollider()->getAABB();
  };
  auto sortedX =
      make_runtime_array<BoundingBox>(list | std::views::transform(toBox));
  std::ranges::sort(sortedX, std::less{},
                    [](const BoundingBox &b) { return b.median().x; });
  auto sortedY =
      make_runtime_array<BoundingBox>(list | std::views::transform(toBox));
  std::ranges::sort(sortedY, std::less{},
                    [](const BoundingBox &b) { return b.median().y; });

  auto arr = make_runtime_array<T *>(list);
  auto root = std::make_unique<Node>();
  topDownRecurse(root.get(), {arr});
  return {std::move(root)};
}
void BoundingVolumeHierarchy::topDownRecurse(Node *const node,
                                             std::span<T *> objects) {
  for (const auto o : objects)
    node->box.expand(o->getCollider()->getAABB());
  if (objects.size() > MAX_OBJECTS_PER_LEAF) {
    auto left = std::make_unique<Node>(), right = std::make_unique<Node>();
    left->parent = node;
    right->parent = node;
    auto split =
        std::partition(objects.begin(), objects.end(), [](const T *const o) {
          return o->getCollider()->getAABB().median().x < 0;
        });
    topDownRecurse(left.get(), {objects.begin(), split});
    topDownRecurse(right.get(), {split, objects.end()});
  } else {
    node->setArray(objects);
  }
}