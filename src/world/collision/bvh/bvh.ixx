module;

#include <algorithm>
#include <cmath>
#include <ranges>

export module bvh;

import <vector>;
import aabb;
import debug;

export namespace bvh {
struct tree_t {
  using obj_list = std::vector<aabb_t>;

  obj_list objs;

  static constexpr unsigned int MAX_OBJECTS_PER_LEAF = 1;
  struct node_t {
    aabb_t box{};
    int left = -1, right = -1;
    unsigned int begin, end;

    bool isLeaf() const { return end - begin == 0; }
  };
  auto getBegin(node_t &node) { return objs.begin() + node.begin; }
  auto getEnd(node_t &node) { return objs.begin() + node.end; }
  auto getObjects(node_t &node) {
    return std::ranges::subrange{getBegin(node), getEnd(node)};
  }

  std::vector<node_t> nodes{};

  tree_t(const std::vector<aabb_t> &o) : objs{o} {
    const std::size_t upperLeafSize = std::bit_ceil(o.size());
    // upper bound of tree size = size of perfect tree
    nodes.reserve(2 * upperLeafSize - 1);

    // nodes.resize(2 * o.size() - 1);
    // leaf nodes = ceil(o.size / MAX_OBJECTS_PER_LEAF)

    // nodes.resize(
    //    2 * static_cast<std::size_t>(std::ceil(static_cast<double>(o.size()) /
    //                                           MAX_OBJECTS_PER_LEAF)) -
    //    1);
  }

  aabb_t computeBounds(obj_list::iterator begin, obj_list::iterator end) {
    aabb_t out{};
    for (const aabb_t &obj : objs)
      out.expand(obj);
    return out;
  }

  obj_list::iterator partitionNode(node_t &node) {
    const glm::vec2 bounds = node.box.size();
    // largest axis: 0 = x>y 1 = x<y
    const bool axis = bounds.x < bounds.y;
    const float middle = bounds[axis] / 2;
    return std::partition(getBegin(node), getEnd(node),
                          [axis, middle](const aabb_t &obj) {
                            return obj.median()[axis] < middle;
                          });
  }

  void topDown() { topDownRecurse(0, objs.begin(), objs.end()); }
  void topDownRecurse(const std::size_t nodeIndex, obj_list::iterator begin,
                      obj_list::iterator end) {
    // construct nodes?
    node_t &working_node = nodes[nodeIndex];

    const std::size_t count = end - begin;
    working_node.box = computeBounds(begin, end);
    if (count <= MAX_OBJECTS_PER_LEAF) {
      working_node.begin =
          static_cast<unsigned int>(std::distance(objs.begin(), begin));
      working_node.end =
          static_cast<unsigned int>(std::distance(objs.begin(), end));
    } else {
      working_node.left = static_cast<int>(nodeIndex * 2 + 1);
      working_node.right = static_cast<int>(nodeIndex * 2 + 2);
      const obj_list::iterator part = partitionNode(working_node);
      topDownRecurse(working_node.left, objs.begin(), part);
      topDownRecurse(working_node.right, part, objs.end());
    }
  }

  void print() const { println(std::log2(nodes.size() + 1)); }
};
}; // namespace bvh