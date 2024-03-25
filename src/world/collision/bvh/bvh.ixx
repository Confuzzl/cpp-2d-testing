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

  static constexpr unsigned int MAX_OBJECTS_PER_LEAF = 4;
  struct node_t {
    aabb_t box{};
    int left = -1, right = -1;
    obj_list::iterator begin, end;

    auto objects() { return std::ranges::subrange{begin, end}; }

    bool isLeaf() const { return end - begin == 0; }
  };
  std::vector<node_t> nodes{};

  tree_t(const std::vector<aabb_t> &o) : objs{o} {
    nodes.resize(2 * o.size() + 1);
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
    return std::partition(node.begin, node.end,
                          [axis, middle](const aabb_t &obj) {
                            return obj.median()[axis] < middle;
                          });
  }

  void topDown() { topDownRecurse(0, objs.begin(), objs.end()); }
  void topDownRecurse(const std::size_t nodeIndex, obj_list::iterator begin,
                      obj_list::iterator end) {
    node_t &working_node = nodes[nodeIndex];
    const std::size_t count = end - begin;
    working_node.box = computeBounds(begin, end);
    if (count <= MAX_OBJECTS_PER_LEAF) {
      working_node.begin = begin;
      working_node.end = end;
    } else {
      working_node.left = static_cast<int>(nodeIndex * 2 + 1);
      working_node.right = static_cast<int>(nodeIndex * 2 + 2);
      const obj_list::iterator part = partitionNode(working_node);
      topDownRecurse(working_node.left, objs.begin(), part);
      topDownRecurse(working_node.right, part, objs.end());
    }
  }

  void print() const {
    const std::size_t depth =
        static_cast<std::size_t>(std::log2(nodes.size() - 1));
    println(depth);
  }
};
}; // namespace bvh