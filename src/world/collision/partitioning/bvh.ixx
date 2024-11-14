module;

#include <ranges>

export module bvh;

import <vector>;
import aabb;
import debug;
import glm;

import color;

export namespace bvh {
struct Tree {
  using obj_t = BoundingBox;
  using obj_list = std::vector<obj_t>;

  obj_list objs;

  static constexpr unsigned int MAX_OBJECTS_PER_LEAF = 2;
  struct Node {
    BoundingBox box{};
    int left = -1, right = -1;
    unsigned int begin = 0, end = 0;

    unsigned int depth = 0;

    bool isLeaf() const;
  };
  obj_list::iterator getBegin(const Node &node);
  obj_list::iterator getEnd(const Node &node);

  auto getObjects(const Node &node) {
    return std::ranges::subrange{getBegin(node), getEnd(node)};
  }

  std::vector<Node> nodes{};
  unsigned int maxDepth = 0;

  static constexpr bool debugging = false;
  template <typename... Args> void debug(Args &&...args) {
    if constexpr (debugging)
      println(std::forward<Args>(args)...);
  }

  Tree() = default;
  Tree(const std::vector<BoundingBox> &o);

  BoundingBox computeBounds(const obj_list::iterator begin,
                            const obj_list::iterator end) const;

  obj_list::iterator partitionNode(Node &node);

  void topDown();
  void topDownRecurse(const std::size_t nodeIndex,
                      const obj_list::iterator begin,
                      const obj_list::iterator end, std::size_t &nodeCount,
                      const unsigned int depth);

  obj_t *queryFirst(const BoundingBox &query, const std::size_t nodeIndex = 0);
  std::vector<obj_t *> queryAll(const BoundingBox &query);
  void queryAllRecurse(std::vector<obj_t *> &list, const BoundingBox &query,
                       const std::size_t nodeIndex);

  void removeNode();
  void removeObject();

  void print() const;
};
}; // namespace bvh