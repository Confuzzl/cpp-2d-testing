export module quadtree;

import <vector>;
import aabb;
import free_list;
import small_vector;

export namespace collision {

// https://stackoverflow.com/questions/41946007
struct Quadtree {
  using index_t = /*std::size_t*/ unsigned int;

  static constexpr index_t NULL_INDEX = -1;

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

    Element() = default;
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
  free_list<ElementNode> elementNodes;
  std::vector<Node> nodes;
  index_t elementCount = 0;
  index_t elementNodeCount = 0;
  index_t firstFourFreeNodes = NULL_INDEX;

  unsigned int MAX_CHILDREN;
  unsigned int MAX_DEPTH;
  BoundingBox BOUNDS;

public:
  Quadtree(const unsigned int maxChildren, unsigned int maxDepth,
           const float radius);

  void insert(const std::size_t ent, const BoundingBox &box);

private:
  void insert(const std::size_t ent, const BoundingBox &box,
              const index_t nodeIndex, const BoundingBox &nodeBox,
              unsigned int depth, index_t elementIndex);

public:
  // returns true on successful remove
  bool remove(const std::size_t ent, const BoundingBox &box);

private:
  index_t remove(const std::size_t ent, const BoundingBox &box, Node &node,
                 const BoundingBox &nodeBox);

public:
  // node list doesnt change size
  void cleanup();

  bool query(const BoundingBox &box) const;

private:
  bool query(const BoundingBox &box, const Node &node,
             const BoundingBox &nodeBox) const;

public:
  // (list, min, max)
  std::tuple<small_vector<index_t>, std::size_t, std::size_t>
  queryLeaves(const BoundingBox &box) const;
  void queryLeaves(const BoundingBox &box, const Node &node,
                   const BoundingBox &nodeBox, small_vector<index_t> &list,
                   std::size_t &min, std::size_t &max) const;

public:
  small_vector<Element> queryAll(const BoundingBox &box) const;
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