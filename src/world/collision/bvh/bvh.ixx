module;

#include <algorithm>
#include <cmath>
#include <ranges>

export module bvh;

import <vector>;
import aabb;
import debug;
import glm;

import color;

export namespace bvh {
struct tree_t {
  using obj_list = std::vector<aabb_t>;

  obj_list objs;

  static constexpr unsigned int MAX_OBJECTS_PER_LEAF = 1;
  struct node_t {
    aabb_t box{};
    int left = -1, right = -1;
    unsigned int begin = 0, end = 0;

    unsigned int depth = 0;

    bool isLeaf() const { return begin != end; }
  };
  auto getBegin(node_t &node) { return objs.begin() + node.begin; }
  auto getEnd(node_t &node) { return objs.begin() + node.end; }
  auto getObjects(node_t &node) {
    return std::ranges::subrange{getBegin(node), getEnd(node)};
  }

  std::vector<node_t> nodes{};
  unsigned int maxDepth = 0;

  bool debugging = false;

  template <typename... Args>
  void debug(std::format_string<Args...> str, Args &&...args) {
    if (debugging)
      println(std::format(str, std::forward<Args>(args)...));
  }
  template <typename T> void debug(const T &s) { debug("{}", s); }

  tree_t(const std::vector<aabb_t> &o) : objs{o} {
    const std::size_t nodeCount = o.size() / MAX_OBJECTS_PER_LEAF;
    const std::size_t upperLeafSize = std::bit_ceil(nodeCount);
    const std::size_t upperNodeCount = 2 * upperLeafSize - 1;
    debug(upperNodeCount);
    // upper bound of tree size = size of perfect tree
    nodes.resize(upperNodeCount);
  }

  aabb_t computeBounds(const obj_list::iterator begin,
                       const obj_list::iterator end) {
    aabb_t out{};
    for (const aabb_t &obj : std::ranges::subrange{begin, end})
      out.expand(obj);
    return out;
  }

  obj_list::iterator partitionNode(node_t &node) {
    debug("\tpartitioning");
    const glm::vec2 size = node.box.size();
    // debug("\t\tsize: {}", vec_string(size));
    //  largest axis: 0 = x>y 1 = x<y
    const bool axis = size.x < size.y;
    const float middle = node.box.median()[axis];
    debug("\t\tmiddle: {:d} {}", axis, middle);
    return std::partition(getBegin(node), getEnd(node),
                          [axis, middle](const aabb_t &obj) {
                            // debug("\t\t\tmed: {:+.2f}",
                            // obj.median()[axis]);
                            return obj.median()[axis] < middle;
                          });
  }

  void topDown() {
    std::size_t nodeCount = 1;
    topDownRecurse(0, objs.begin(), objs.end(), nodeCount, 0);
    nodes.resize(nodeCount);

    std::sort(nodes.begin(), nodes.end(), [](const node_t &a, const node_t &b) {
      return a.depth < b.depth;
    });
    maxDepth = nodes.back().depth;
    // maxDepth = nodes[]
  }
  void topDownRecurse(const std::size_t nodeIndex,
                      const obj_list::iterator begin,
                      const obj_list::iterator end, std::size_t &nodeCount,
                      const unsigned int depth) {
    debug("working node: {}", nodeIndex);
    debug("\tit: {} {}", std::distance(objs.begin(), begin),
          std::distance(objs.begin(), end));

    const std::size_t count = end - begin;
    debug("\tcount: {}", count);
    if (count == 0)
      return;

    node_t &working_node = nodes[nodeIndex];

    working_node.depth = depth;

    working_node.box = computeBounds(begin, end);
    debug("\tbox: {}", working_node.box);

    working_node.begin =
        static_cast<unsigned int>(std::distance(objs.begin(), begin));
    working_node.end =
        static_cast<unsigned int>(std::distance(objs.begin(), end));
    debug("\tnode: [{} {}]", working_node.begin, working_node.end);

    if (count <= MAX_OBJECTS_PER_LEAF) {

    } else {
      working_node.left = static_cast<int>(nodeCount++);
      working_node.right = static_cast<int>(nodeCount++);
      const obj_list::iterator part = partitionNode(working_node);
      working_node.begin = 0;
      working_node.end = 0;
      debug("\tleft: {} right: {}", std::distance(begin, part),
            std::distance(part, end));
      topDownRecurse(working_node.left, begin, part, nodeCount, depth + 1);
      topDownRecurse(working_node.right, part, end, nodeCount, depth + 1);
    }
  }

  void print() const {
    println(nodes.size());

    for (int i = 0; i < nodes.size(); i++) {
      const node_t &node = nodes[i];
      println("[{}] {}: [{},{}] [{},{}] {}", i,
              node.isLeaf() ? "leaf" : "branch", node.left, node.right,
              node.begin, node.end, node.depth);
    }
  }
};
}; // namespace bvh