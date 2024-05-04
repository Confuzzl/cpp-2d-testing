module;

#include <algorithm>
#include <cmath>

module bvh;

using namespace bvh;

bool tree_t::node_t::isLeaf() const { return begin != end; }

tree_t::obj_list::iterator tree_t::getBegin(const node_t &node) {
  return objs.begin() + node.begin;
}
tree_t::obj_list::iterator tree_t::getEnd(const node_t &node) {
  return objs.begin() + node.end;
}

tree_t::tree_t(const std::vector<aabb_t> &o) : objs{o} {
  const std::size_t nodeCount = o.size() / MAX_OBJECTS_PER_LEAF;
  const std::size_t upperLeafSize = std::bit_ceil(nodeCount);
  const std::size_t upperNodeCount = 2 * upperLeafSize - 1;
  debug(upperNodeCount);
  // upper bound of tree size = size of perfect tree
  nodes.resize(upperNodeCount);
}

aabb_t tree_t::computeBounds(const obj_list::iterator begin,
                             const obj_list::iterator end) const {
  aabb_t out{};
  for (const aabb_t &obj : std::ranges::subrange{begin, end})
    out.expand(obj);
  return out;
}

tree_t::obj_list::iterator tree_t::partitionNode(node_t &node) {
  enum Axis : bool { X = false, Y = true };

  debug("\tpartitioning");
  const glm::vec2 size = node.box.size();
  debug("\t\tsize: {}", vec_string(size));

  const Axis axis = size.x > size.y ? Axis::X : Axis::Y;
  const float middle = node.box.median()[axis];
  debug("\t\tmiddle: {:d} {}", static_cast<bool>(axis), middle);
  return std::partition(getBegin(node), getEnd(node),
                        [axis, middle](const aabb_t &obj) {
                          // debug("\t\t\tmed: {:+.2f}",
                          // obj.median()[axis]);
                          return obj.median()[axis] < middle;
                        });
}

void tree_t::topDown() {
  std::size_t nodeCount = 1;
  topDownRecurse(0, objs.begin(), objs.end(), nodeCount, 0);
  nodes.resize(nodeCount);

  std::sort(nodes.begin(), nodes.end(),
            [](const node_t &a, const node_t &b) { return a.depth < b.depth; });
  maxDepth = nodes.back().depth;
}
void tree_t::topDownRecurse(const std::size_t nodeIndex,
                            const obj_list::iterator begin,
                            const obj_list::iterator end,
                            std::size_t &nodeCount, const unsigned int depth) {
  debug("working node: {}", nodeIndex);
  debug("\tit: {} {}", std::distance(objs.begin(), begin),
        std::distance(objs.begin(), end));

  const std::size_t count = end - begin;
  debug("\tcount: {}", count);

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

    const std::size_t l = part - begin, r = end - part;
    debug("\tleft: {} right: {}", l, r);
    // empty side dont part
    if (l == 0 || r == 0) {
      debug("\tearly abort");
      return;
    }

    working_node.begin = 0;
    working_node.end = 0;

    topDownRecurse(working_node.left, begin, part, nodeCount, depth + 1);
    topDownRecurse(working_node.right, part, end, nodeCount, depth + 1);
  }
}

void tree_t::print() const {
  println(nodes.size());

  for (int i = 0; i < nodes.size(); i++) {
    const node_t &node = nodes[i];
    println("[{}] {}: [{},{}] [{},{}] {}", i, node.isLeaf() ? "leaf" : "branch",
            node.left, node.right, node.begin, node.end, node.depth);
  }
}
