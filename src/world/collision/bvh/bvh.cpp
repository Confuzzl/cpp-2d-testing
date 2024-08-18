module;

#include <algorithm>
#include <cmath>

module bvh;

using namespace bvh;

bool Tree::Node::isLeaf() const { return begin != end; }

Tree::obj_list::iterator Tree::getBegin(const Node &node) {
  return objs.begin() + node.begin;
}
Tree::obj_list::iterator Tree::getEnd(const Node &node) {
  return objs.begin() + node.end;
}

Tree::Tree(const std::vector<BoundingBox> &o) : objs{o} {
  nodes.resize(2 * o.size() - 1);
}

BoundingBox Tree::computeBounds(const obj_list::iterator begin,
                                const obj_list::iterator end) const {
  BoundingBox out{};
  for (const BoundingBox &obj : std::ranges::subrange{begin, end})
    out.expand(obj);
  return out;
}

Tree::obj_list::iterator Tree::partitionNode(Node &node) {
  enum Axis : bool { X = false, Y = true };

  debug("\tpartitioning");
  const glm::vec2 size = node.box.size();
  debug("\t\tsize: {}", vec_string(size));

  const Axis axis = size.x > size.y ? Axis::X : Axis::Y;
  const float middle = node.box.median()[axis];
  debug("\t\tmiddle: {:d} {}", static_cast<bool>(axis), middle);
  return std::partition(getBegin(node), getEnd(node),
                        [axis, middle](const BoundingBox &obj) {
                          // debug("\t\t\tmed: {:+.2f}",
                          // obj.median()[axis]);
                          return obj.median()[axis] < middle;
                        });
}

void Tree::topDown() {
  std::size_t nodeCount = 1;
  topDownRecurse(0, objs.begin(), objs.end(), nodeCount, 0);
  nodes.resize(nodeCount);

  std::sort(nodes.begin(), nodes.end(),
            [](const Node &a, const Node &b) { return a.depth < b.depth; });
  maxDepth = nodes.back().depth;
}
void Tree::topDownRecurse(const std::size_t nodeIndex,
                          const obj_list::iterator begin,
                          const obj_list::iterator end, std::size_t &nodeCount,
                          const unsigned int depth) {
  debug("working node: {}", nodeIndex);
  debug("\tit: {} {}", std::distance(objs.begin(), begin),
        std::distance(objs.begin(), end));

  const std::size_t count = end - begin;
  debug("\tcount: {}", count);

  Node &working_node = nodes[nodeIndex];

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

Tree::obj_t *Tree::queryFirst(const BoundingBox &query,
                              const std::size_t nodeIndex) {
  const Node &node = nodes[nodeIndex];
  if (!query.intersects(node.box))
    return nullptr;

  if (node.isLeaf()) {
    for (obj_t &obj : getObjects(node))
      if (obj.intersects(query))
        return &obj;
    return nullptr;
  }

  if (obj_t *left = queryFirst(query, node.left); left)
    return left;
  if (obj_t *right = queryFirst(query, node.right); right)
    return right;
  return nullptr;
}
std::vector<Tree::obj_t *> Tree::queryAll(const BoundingBox &query) {
  std::vector<Tree::obj_t *> list{};
  queryAllRecurse(list, query, 0);
  return list;
}
void Tree::queryAllRecurse(std::vector<Tree::obj_t *> &list,
                           const BoundingBox &query,
                           const std::size_t nodeIndex) {
  const Node &node = nodes[nodeIndex];

  if (node.isLeaf()) {
    for (obj_t &obj : getObjects(node))
      if (obj.intersects(query))
        list.emplace_back(&obj);
  } else {
    queryAllRecurse(list, query, node.left);
    queryAllRecurse(list, query, node.right);
  }
}

void Tree::print() const {
  println(nodes.size());

  for (auto i = 0u; i < nodes.size(); i++) {
    const Node &node = nodes[i];
    println("[{}] {}: [{},{}] [{},{}] {}", i, node.isLeaf() ? "leaf" : "branch",
            node.left, node.right, node.begin, node.end, node.depth);
  }
}
