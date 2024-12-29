export module quadtree;

import <vector>;
import aabb;
import free_list;

export namespace collision {

// https://stackoverflow.com/questions/41946007
struct Quadtree {
  using index_t = /*std::size_t*/ unsigned int;

  static constexpr index_t NULL_INDEX = -1;
  static constexpr auto MAX_CHILDREN = 2;
  static constexpr auto MAX_DEPTH = 8u;
  static constexpr auto RADIUS = 1 << 2;
  static constexpr BoundingBox BOUNDS{{-RADIUS, -RADIUS}, {RADIUS, RADIUS}};

  struct Node {
    index_t first;
    index_t count;

    Node(const index_t first, const index_t count)
        : first{first}, count{count} {}

    bool isLeaf() const { return count != NULL_INDEX; }
  };

  struct Element {
    std::size_t ent;
    BoundingBox box;

    Element(const std::size_t ent, const BoundingBox &box)
        : ent{ent}, box{box} {}
  };
  struct ElementNode {
    index_t next;
    index_t elementIndex;

    ElementNode(const index_t next, const index_t elementIndex)
        : next{next}, elementIndex{elementIndex} {}
  };

  free_list<Element> elements;
  std::size_t elementCount = 0;
  free_list<ElementNode> elementNodes;
  std::size_t elementNodeCount = 0;
  std::vector<Node> nodes;

  index_t firstFourFreeNodes = NULL_INDEX;

  Quadtree();

  void insert(const std::size_t ent, const BoundingBox &box);
  void insert(const std::size_t ent, const BoundingBox &box,
              const index_t nodeIndex, const BoundingBox &nodeBox,
              unsigned int depth, index_t elementIndex);
  // returns true on successful remove
  bool remove(const std::size_t ent, const BoundingBox &box);
  bool remove(const std::size_t ent, const BoundingBox &box, Node &node,
              const BoundingBox &nodeBox);

  void cleanup();
};
} // namespace collision

import <format>;

export template <> struct std::formatter<collision::Quadtree::Node> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
  auto format(const collision::Quadtree::Node &node,
              std::format_context &ctx) const {
    return std::format_to(ctx.out(), "Node(first={}, count={})", node.first,
                          node.count);
  }
};