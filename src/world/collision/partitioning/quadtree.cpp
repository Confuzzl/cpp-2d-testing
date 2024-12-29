module;

#define DEBUG

#ifdef DEBUG
#define debugln(...) println(__VA_ARGS__)
#else
#define debugln(...)
#endif

module quadtree;

using namespace collision;

import small_vector;
import debug;

static auto quadrants(const BoundingBox &nodeBox) {
  struct Boxes {
    BoundingBox tlBox, trBox, blBox, brBox;
  };
  const auto halfBounds = nodeBox.size() / 2.0f;
  const glm::vec2 starts[4] = {
      nodeBox.min, nodeBox.min + glm::vec2{halfBounds.x, 0},
      nodeBox.min + glm::vec2{0, halfBounds.y}, nodeBox.min + halfBounds};
  return Boxes{.tlBox = BoundingBox::startSize(starts[2], halfBounds),
               .trBox = BoundingBox::startSize(starts[3], halfBounds),
               .blBox = BoundingBox::startSize(starts[0], halfBounds),
               .brBox = BoundingBox::startSize(starts[1], halfBounds)};
}

Quadtree::Quadtree() { auto &root = nodes.emplace_back(0, 0); }

void Quadtree::insert(const std::size_t ent, const BoundingBox &box) {
  insert(ent, box, 0, BOUNDS, MAX_DEPTH, NULL_INDEX);
}
void Quadtree::insert(const std::size_t ent, const BoundingBox &box,
                      const index_t nodeIndex, const BoundingBox &nodeBox,
                      unsigned int depth, index_t elementIndex) {
  if (!box.intersects(nodeBox))
    return;

  // insert element once for root
  if (depth == MAX_DEPTH) {
    elementIndex = static_cast<index_t>(elements.emplace_back(ent, box));
    elementCount++;
  }

  Node &node = nodes[nodeIndex];

  debugln("Quadtree::insert(\n"
          "\tent={},\n"
          "\tbox={},\n"
          "\tnodeIndex={} {},\n"
          "\tnodeBox={},\n"
          "\tdepth={},\n"
          "\telementIndex={}\n"
          ")",
          ent, box, nodeIndex, node, nodeBox, depth, elementIndex);

  if (node.isLeaf() && (node.count < MAX_CHILDREN || depth == 0)) {
    if (node.count == 0) {
      node.first = static_cast<index_t>(
          elementNodes.emplace_back(NULL_INDEX, elementIndex));
      elementNodeCount++;
      debugln("empty leaf new first {}: ({}, {})", node.first, NULL_INDEX,
              elementIndex);
    } else {
      debugln("nonempty leaf");
      index_t tailElementNodeIndex = node.first,
              next = elementNodes[tailElementNodeIndex].next;
      // cant use pointer because of free_list::data resizing
      while (next != NULL_INDEX) {
        tailElementNodeIndex = next;
        next = elementNodes[tailElementNodeIndex].next;
      }
      elementNodes[tailElementNodeIndex].next = static_cast<index_t>(
          elementNodes.emplace_back(NULL_INDEX, elementIndex));
      elementNodeCount++;
    }
    node.count++;
    return;
  }

  const Node oldNode = node;

  depth--;
  node.count = NULL_INDEX;
  const auto [tlBox, trBox, blBox, brBox] = quadrants(nodeBox);
  if (oldNode.isLeaf()) {
    if (firstFourFreeNodes == NULL_INDEX) {
      node.first = static_cast<index_t>(nodes.size());
      nodes.emplace_back(NULL_INDEX, 0);
      nodes.emplace_back(NULL_INDEX, 0);
      nodes.emplace_back(NULL_INDEX, 0);
      nodes.emplace_back(NULL_INDEX, 0);
    } else {
      debugln("inserting with firstfourfreenodes");
      node.first = firstFourFreeNodes;
      firstFourFreeNodes = NULL_INDEX;
      nodes[node.first + 0] = {NULL_INDEX, 0};
      nodes[node.first + 1] = {NULL_INDEX, 0};
      nodes[node.first + 2] = {NULL_INDEX, 0};
      nodes[node.first + 3] = {NULL_INDEX, 0};
    }
  }

  // previous reference may be invalidated from nodes vector emplacing
  Node &newNode = nodes[nodeIndex];
  const index_t tl = newNode.first + 0, tr = newNode.first + 1,
                bl = newNode.first + 2, br = newNode.first + 3;

  // push old elements down
  if (oldNode.isLeaf()) {
    debugln("pushing leaves");
    index_t elementNodeIndex = oldNode.first,
            next = elementNodes[elementNodeIndex].next;
    auto count = 0u;
    while (count++ < oldNode.count) {
      const ElementNode &elementNode = elementNodes[elementNodeIndex];
      const index_t index = elementNode.elementIndex;
      const Element &element = elements[index];
      debugln("{} (index={}) -> {}", elementNodeIndex, index, next);
      elementNodes.erase(elementNodeIndex);
      insert(element.ent, element.box, tl, tlBox, depth, index);
      insert(element.ent, element.box, tr, trBox, depth, index);
      insert(element.ent, element.box, bl, blBox, depth, index);
      insert(element.ent, element.box, br, brBox, depth, index);

      elementNodeIndex = next;
      if (count != oldNode.count)
        next = elementNodes[elementNodeIndex].next;
    }
  }
  debugln("inserting children");
  insert(ent, box, tl, tlBox, depth, elementIndex);
  insert(ent, box, tr, trBox, depth, elementIndex);
  insert(ent, box, bl, blBox, depth, elementIndex);
  insert(ent, box, br, brBox, depth, elementIndex);
}
bool Quadtree::remove(const std::size_t ent, const BoundingBox &box) {
  return remove(ent, box, nodes[0], BOUNDS);
}
bool Quadtree::remove(const std::size_t ent, const BoundingBox &box, Node &node,
                      const BoundingBox &nodeBox) {
  if (!box.intersects(nodeBox))
    return false;

  debugln("Quadtree::remove(\n"
          "\tent={},\n"
          "\tbox={},\n"
          "\tnode={},\n"
          "\tnodeBox={}\n"
          ")",
          ent, box, node, nodeBox);

  if (!node.isLeaf()) {
    debugln("removing children");

    const auto [tlBox, trBox, blBox, brBox] = quadrants(nodeBox);
    if (remove(ent, box, nodes[node.first + 0], tlBox) ||
        remove(ent, box, nodes[node.first + 1], trBox) ||
        remove(ent, box, nodes[node.first + 2], blBox) ||
        remove(ent, box, nodes[node.first + 3], brBox))
      return true;
    return false;
  }
  if (node.count == 0)
    return false;

  ElementNode dummy{node.first, NULL_INDEX};
  index_t prevIndex = NULL_INDEX, currIndex = node.first;
  auto count = 0u;
  while (count < node.count) {
    auto &prevElementNode = (count == 0 ? dummy : elementNodes[prevIndex]),
         &currElementNode = elementNodes[currIndex];

    debugln("{} ent: {}", count, currElementNode.elementIndex);
    if (const auto index = currElementNode.elementIndex;
        index != NULL_INDEX && elements[index].ent == ent) {
      debugln("found ent");
      elements.erase(index);
      elementCount--;
      prevElementNode.next = currElementNode.next;
      elementNodes.erase(currIndex);
      elementNodeCount--;

      if (count == 0)
        node.first = prevElementNode.next;

      node.count--;
      debugln("new count {}", node.count);
      if (node.count == 0)
        node.first = NULL_INDEX;
      return true;
    }
    prevIndex = currIndex;
    if (count != node.count)
      currIndex = currElementNode.next;

    count++;
  }

  return false;
}

void Quadtree::cleanup() {
  // debugln("Quadtree::cleanup()");
  small_vector<index_t> processing;
  if (!nodes[0].isLeaf())
    processing.emplace_back(0);

  while (processing.size() > 0) {
    Node &node = nodes[processing.back()];
    processing.pop();

    auto empties = 0u;
    for (auto i = 0u; i < 4; i++) {
      const index_t childIndex = node.first + i;
      const Node &child = nodes[childIndex];
      if (child.count == 0)
        empties++;
      else if (!child.isLeaf())
        processing.emplace_back(childIndex);
    }
    if (empties != 4)
      continue;

    nodes[node.first].first = firstFourFreeNodes;
    firstFourFreeNodes = node.first;
    node.first = NULL_INDEX;
    node.count = 0;

    debugln("freed quad {}", firstFourFreeNodes);
  }
}