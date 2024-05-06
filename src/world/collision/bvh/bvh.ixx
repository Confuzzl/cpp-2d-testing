module;

#include <ranges>

export module bvh;

import <vector>;
import aabb;
import debug;
import glm;

import color;

export namespace bvh {
struct tree_t {
  using obj_t = aabb_t;
  using obj_list = std::vector<obj_t>;

  obj_list objs;

  static constexpr unsigned int MAX_OBJECTS_PER_LEAF = 2;
  struct node_t {
    aabb_t box{};
    int left = -1, right = -1;
    unsigned int begin = 0, end = 0;

    unsigned int depth = 0;

    bool isLeaf() const;
  };
  obj_list::iterator getBegin(const node_t &node);
  obj_list::iterator getEnd(const node_t &node);
  // obj_list::const_iterator getBegin(const node_t &node) const;
  // obj_list::const_iterator getEnd(const node_t &node) const;

  auto getObjects(const node_t &node) {
    return std::ranges::subrange{getBegin(node), getEnd(node)};
  }
  // const auto getObjects(const node_t &node) const {
  //   return std::ranges::subrange{getBegin(node), getEnd(node)};
  // }

  std::vector<node_t> nodes{};
  unsigned int maxDepth = 0;

  bool debugging = true;

  template <typename... Args>
  void debug(std::format_string<Args...> str, Args &&...args) {
    if (debugging)
      println(std::format(str, std::forward<Args>(args)...));
  }
  template <typename T> void debug(const T &s) { debug("{}", s); }

  tree_t() = default;
  tree_t(const std::vector<aabb_t> &o);

  aabb_t computeBounds(const obj_list::iterator begin,
                       const obj_list::iterator end) const;

  obj_list::iterator partitionNode(node_t &node);

  void topDown();
  void topDownRecurse(const std::size_t nodeIndex,
                      const obj_list::iterator begin,
                      const obj_list::iterator end, std::size_t &nodeCount,
                      const unsigned int depth);

  obj_t *queryFirst(const aabb_t &query, const std::size_t nodeIndex = 0);
  std::vector<obj_t *> queryAll(const aabb_t &query);
  void queryAllRecurse(std::vector<obj_t *> &list, const aabb_t &query,
                       const std::size_t nodeIndex);

  void removeNode();
  void removeObject();

  void print() const;
};
}; // namespace bvh