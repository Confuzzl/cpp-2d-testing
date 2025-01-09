module quadtree;

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

using namespace collision;

static void decElementNodes(Quadtree *self) {
  if (self->elementNodeCount == 0)
    throw std::runtime_error{"AAAAAAAAAAAAA"};
  self->elementNodeCount--;
}
static void decElements(Quadtree *self) {
  if (self->elementCount == 0)
    throw std::runtime_error{"BBBBBBBBBBBB"};
  self->elementCount--;
}

static constexpr bool print = false;

Quadtree::Quadtree() { auto &root = nodes.emplace_back(0, 0); }

void Quadtree::insert(const std::size_t ent, const BoundingBox &box) {
  debugln<DEBUG_QUADTREE>(print, "Quadtree::insert()");

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
    debugln<DEBUG_QUADTREE>(print, "element {} at index {}", ent, elementIndex);
    elementCount++;
  }

  Node &node = nodes[nodeIndex];

  debugln<DEBUG_QUADTREE>(print,
                          "Quadtree::insert(\n"
                          "\tent={},\n"
                          "\tbox={},\n"
                          "\tnodeIndex={} {},\n"
                          "\tnodeBox={},\n"
                          "\tdepth={},\n"
                          "\telementIndex={}\n"
                          ")",
                          ent, box, nodeIndex, node, nodeBox, depth,
                          elementIndex);

  if (node.isLeaf() && (node.count < MAX_CHILDREN || depth == 0)) {
    if (node.count == 0) {
      node.first = static_cast<index_t>(
          elementNodes.emplace_back(NULL_INDEX, elementIndex));
      elementNodeCount++;
      debugln<DEBUG_QUADTREE>(print, "empty leaf new first {}: ({}, {})",
                              node.first, NULL_INDEX, elementIndex);
    } else {
      debugln<DEBUG_QUADTREE>(print, "nonempty leaf");
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
      debugln<DEBUG_QUADTREE>(print, "inserting with firstfourfreenodes");
      node.first = firstFourFreeNodes;
      const auto nextFree = nodes[node.first].first;
      nodes[node.first + 0] = {NULL_INDEX, 0};
      nodes[node.first + 1] = {NULL_INDEX, 0};
      nodes[node.first + 2] = {NULL_INDEX, 0};
      nodes[node.first + 3] = {NULL_INDEX, 0};
      firstFourFreeNodes = nextFree;
    }
  }

  // previous reference may be invalidated from nodes vector emplacing
  Node &newNode = nodes[nodeIndex];
  const index_t tl = newNode.first + 0, tr = newNode.first + 1,
                bl = newNode.first + 2, br = newNode.first + 3;

  // push old elements down
  if (oldNode.isLeaf()) {
    debugln<DEBUG_QUADTREE>(print, "pushing leaves");
    index_t elementNodeIndex = oldNode.first,
            next = elementNodes[elementNodeIndex].next;
    auto count = 0u;
    while (count++ < oldNode.count) {
      const ElementNode &elementNode = elementNodes[elementNodeIndex];
      const index_t index = elementNode.elementIndex;
      const Element &element = elements[index];
      debugln<DEBUG_QUADTREE>(print, "{} (index={}) -> {}", elementNodeIndex,
                              index, next);
      elementNodes.erase(elementNodeIndex);
      decElementNodes(this);
      insert(element.ent, element.box, tl, tlBox, depth, index);
      insert(element.ent, element.box, tr, trBox, depth, index);
      insert(element.ent, element.box, bl, blBox, depth, index);
      insert(element.ent, element.box, br, brBox, depth, index);

      elementNodeIndex = next;
      if (count != oldNode.count)
        next = elementNodes[elementNodeIndex].next;
    }
  }
  debugln<DEBUG_QUADTREE>(print, "inserting children");
  insert(ent, box, tl, tlBox, depth, elementIndex);
  insert(ent, box, tr, trBox, depth, elementIndex);
  insert(ent, box, bl, blBox, depth, elementIndex);
  insert(ent, box, br, brBox, depth, elementIndex);
}

bool Quadtree::remove(const std::size_t ent, const BoundingBox &box) {
  const auto index = remove(ent, box, nodes[0], BOUNDS);
  if (index != NULL_INDEX) {
    debugln<DEBUG_QUADTREE>(print, "removing element {}", elements[index].ent);
    elements.erase(index);
    decElements(this);
  }
  return index != NULL_INDEX;
}

Quadtree::index_t Quadtree::remove(const std::size_t ent,
                                   const BoundingBox &box, Node &node,
                                   const BoundingBox &nodeBox) {
  if (!box.intersects(nodeBox))
    return NULL_INDEX;

  debugln<DEBUG_QUADTREE>(print,
                          "Quadtree::remove(\n"
                          "\tent={},\n"
                          "\tbox={},\n"
                          "\tnode={},\n"
                          "\tnodeBox={}\n"
                          ")",
                          ent, box, node, nodeBox);

  if (!node.isLeaf()) {
    debugln<DEBUG_QUADTREE>(print, "removing children");

    const auto [tlBox, trBox, blBox, brBox] = quadrants(nodeBox);

    const auto tl = remove(ent, box, nodes[node.first + 0], tlBox),
               tr = remove(ent, box, nodes[node.first + 1], trBox),
               bl = remove(ent, box, nodes[node.first + 2], blBox),
               br = remove(ent, box, nodes[node.first + 3], brBox);

    index_t removalIndex = NULL_INDEX;
    for (const auto index : {tl, tr, bl, br}) {
      if (index == NULL_INDEX)
        continue;
      removalIndex = index;
      // break;
      if (removalIndex != NULL_INDEX && index != removalIndex)
        throw std::runtime_error{"EEEE"};
    }
    debugln<DEBUG_QUADTREE>(print, "removalIndex={}", removalIndex);

    return removalIndex;
  }
  if (node.count == 0)
    return NULL_INDEX;

  ElementNode dummy{node.first, NULL_INDEX};
  index_t prevIndex = NULL_INDEX, currIndex = node.first;
  auto count = 0u;
  while (count < node.count) {
    auto &prevElementNode = (count == 0 ? dummy : elementNodes[prevIndex]),
         &currElementNode = elementNodes[currIndex];

    debugln<DEBUG_QUADTREE>(print, "{}: ent[{}]={}", count,
                            currElementNode.elementIndex,
                            elements[currElementNode.elementIndex].ent);
    if (const auto index = currElementNode.elementIndex;
        index != NULL_INDEX && elements[index].ent == ent) {

      // stitch list
      prevElementNode.next = currElementNode.next;
      elementNodes.erase(currIndex);
      // elementNodeCount--;
      decElementNodes(this);

      // set head if removing first enode
      if (count == 0)
        node.first = prevElementNode.next;

      node.count--;
      debugln<DEBUG_QUADTREE>(print, "new count {}", node.count);
      if (node.count == 0)
        node.first = NULL_INDEX;
      return index;
    }
    prevIndex = currIndex;
    if (count != node.count)
      currIndex = currElementNode.next;

    count++;
  }

  return NULL_INDEX;
}

void Quadtree::cleanup() {
  debugln<DEBUG_QUADTREE>(print, "Quadtree::cleanup()");

  if (nodes[0].isLeaf())
    return;

  small_vector<index_t> processing;
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

    debugln<DEBUG_QUADTREE>(print, "freed quad {}", firstFourFreeNodes);
  }
}
