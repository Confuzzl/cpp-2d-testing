export module quadtree;

import <vector>;
import aabb;

export namespace collision {
struct quadtree {
  static constexpr auto WIDTH = 1u << 16;
  using NodeIndex = unsigned int;
  static constexpr NodeIndex NULL_INDEX = -1;

  struct Node {
    NodeIndex children[4]{NULL_INDEX, NULL_INDEX, NULL_INDEX, NULL_INDEX};
  };

  std::vector<Node> nodes;
  std::vector<std::size_t> entityList;
};
} // namespace collision