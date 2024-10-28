module;

#include <ranges>

module bvh2;

using namespace collision;

import <algorithm>;

BoundingVolumeHierarchy::Node::~Node() {
  if (_isLeaf)
    array.~vector();
  else {
    left.~unique_ptr();
    right.~unique_ptr();
  }
}

bool BoundingVolumeHierarchy::Node::isRoot() const { return !parent; }
bool BoundingVolumeHierarchy::Node::isLeaf() const { return _isLeaf; }
bool BoundingVolumeHierarchy::Node::isBranch() const {
  return !isRoot() && !isLeaf();
}

void BoundingVolumeHierarchy::Node::setArray(std::span<T *> objects) {
  _isLeaf = true;
  array.assign(objects.begin(), objects.end());
}
void BoundingVolumeHierarchy::Node::setChildren(std::unique_ptr<Node> &&left,
                                                std::unique_ptr<Node> &&right) {
  _isLeaf = false;
  left->parent = this;
  right->parent = this;
  this->left = std::move(left);
  this->right = std::move(right);
}

void BoundingVolumeHierarchy::add(const T *const o) {}

BoundingVolumeHierarchy
BoundingVolumeHierarchy::topDown(const std::vector<T *> &list) {
  BoundingVolumeHierarchy out;
  auto arr = make_runtime_array<T *>(list);
  topDownRecurse(&out.root, {arr}, 0);
  return out;
}
void BoundingVolumeHierarchy::topDownRecurse(Node *const node,
                                             std::span<T *> objects,
                                             const int depth) {
  for (const T *const o : objects)
    node->box.expand(*o);

  if (objects.size() <= MAX_OBJECTS_PER_LEAF) {
    node->setArray(objects);
    return;
  }

  const bool axis = node->box.width() < node->box.height();
  const float median = node->box.median()[axis];
  auto split = std::partition(
      objects.begin(), objects.end(),
      [axis, median](const T *const o) { return o->median()[axis] <= median; });
  // partition unable to create 2 groups
  // sort and then split in half
  if (split - objects.begin() == 0 || objects.end() - split == 0) {
    std::sort(objects.begin(), objects.end(),
              [axis](const T *const a, const T *const b) {
                return a->median()[axis] < b->median()[axis];
              });
    split = objects.begin() + objects.size() / 2;
  }

  node->setChildren(std::make_unique<Node>(), std::make_unique<Node>());
  auto left = node->left.get(), right = node->right.get();

  topDownRecurse(left, {objects.begin(), split}, depth + 1);
  topDownRecurse(right, {split, objects.end()}, depth + 1);
}