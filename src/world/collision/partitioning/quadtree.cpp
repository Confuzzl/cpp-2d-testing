module quadtree;

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

static constexpr bool print = 1;

Quadtree::Quadtree(const unsigned int maxChildren, unsigned int maxDepth,
                   const float radius)
    : MAX_CHILDREN{maxChildren}, MAX_DEPTH{maxDepth},
      BOUNDS{glm::vec2{-radius}, glm::vec2{+radius}} {
  auto &root = nodes.emplace_back(0, 0);
}

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
      for (auto i = 0u; i < 4; i++)
        nodes.emplace_back(NULL_INDEX, 0);
    } else {
      debugln<DEBUG_QUADTREE>(print, "inserting with firstfourfreenodes");
      node.first = firstFourFreeNodes;
      const auto nextFree = nodes[node.first].first;
      for (auto i = 0u; i < 4; i++)
        nodes[node.first + i] = {NULL_INDEX, 0};
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
    for (auto count = 0u; count < oldNode.count; count++) {
      const ElementNode &elementNode = elementNodes[elementNodeIndex];
      const index_t index = elementNode.elementIndex;
      const Element &element = elements[index];
      debugln<DEBUG_QUADTREE>(print, "{} (index={}) -> {}", elementNodeIndex,
                              index, next);
      elementNodes.erase(elementNodeIndex);
      decElementNodes(this);
      insert(ent, box, tl, tlBox, depth, index);
      insert(ent, box, tr, trBox, depth, index);
      insert(ent, box, bl, blBox, depth, index);
      insert(ent, box, br, brBox, depth, index);

      elementNodeIndex = next;
      if (elementNodeIndex != NULL_INDEX)
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

    // either all null or some must share the same index
    const auto tl = remove(ent, box, nodes[node.first + 0], tlBox),
               tr = remove(ent, box, nodes[node.first + 1], trBox),
               bl = remove(ent, box, nodes[node.first + 2], blBox),
               br = remove(ent, box, nodes[node.first + 3], brBox);

    index_t removalIndex = NULL_INDEX;
    for (const auto index : {tl, tr, bl, br}) {
      if (index == NULL_INDEX)
        continue;
      removalIndex = index;
      break;
    }
    debugln<DEBUG_QUADTREE>(print, "removalIndex={}", removalIndex);

    return removalIndex;
  }
  if (node.count == 0)
    return NULL_INDEX;

  ElementNode dummy{node.first, NULL_INDEX};
  auto prevElementNode = &dummy, currElementNode = &elementNodes[node.first];
  for (auto count = 0u; count < node.count; count++) {
    auto currIndex = prevElementNode->next;

    debugln<DEBUG_QUADTREE>(print, "{}: ent[{}]={}", count,
                            currElementNode->elementIndex,
                            elements[currElementNode->elementIndex].ent);
    if (const auto index = currElementNode->elementIndex;
        index != NULL_INDEX && elements[index].ent == ent) {

      // stitch list
      prevElementNode->next = currElementNode->next;
      elementNodes.erase(currIndex);
      // elementNodeCount--;
      decElementNodes(this);

      // set head if removing first enode
      if (count == 0)
        node.first = prevElementNode->next;

      node.count--;
      debugln<DEBUG_QUADTREE>(print, "new count {}", node.count);
      if (node.count == 0)
        node.first = NULL_INDEX;
      return index;
    }
    prevElementNode = currElementNode;
    if (const auto next = prevElementNode->next; next != NULL_INDEX)
      currElementNode = &elementNodes[next];
  }
  return NULL_INDEX;
}

void Quadtree::cleanup() {
  // debugln<DEBUG_QUADTREE>(print, "Quadtree::cleanup()");

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

    node = {NULL_INDEX, 0};

    debugln<DEBUG_QUADTREE>(print, "freed quartet {}", firstFourFreeNodes);
  }
}

bool Quadtree::query(const BoundingBox &box) const {
  return query(box, nodes[0], BOUNDS);
}
bool Quadtree::query(const BoundingBox &box, const Node &node,
                     const BoundingBox &nodeBox) const {
  if (!nodeBox.intersects(box))
    return false;
  if (node.isLeaf()) {
    if (node.count == 0)
      return false;

    auto currElementNode = &elementNodes[node.first];
    for (auto i = 0u; i < node.count; i++) {
      auto &element = elements[currElementNode->elementIndex];
      if (element.box.intersects(box))
        return true;
      if (const auto next = currElementNode->next; next != NULL_INDEX)
        currElementNode = &elementNodes[next];
    }
    return false;
  }
  const auto [tlBox, trBox, blBox, brBox] = quadrants(nodeBox);
  const auto tl = query(box, nodes[node.first + 0], tlBox),
             tr = query(box, nodes[node.first + 1], trBox),
             bl = query(box, nodes[node.first + 2], blBox),
             br = query(box, nodes[node.first + 3], brBox);
  return tl || tr || bl || br;
}

std::tuple<small_vector<Quadtree::index_t>, std::size_t, std::size_t>
Quadtree::queryLeaves(const BoundingBox &box) const {
  debugln(true, "Quadtree::queryLeaves(box={})", box);

  small_vector<index_t> out;
  std::size_t min = -1, max = 0;
  queryLeaves(box, nodes[0], BOUNDS, out, min, max);
  return {std::move(out), min, max};
}
void Quadtree::queryLeaves(const BoundingBox &box, const Node &node,
                           const BoundingBox &nodeBox,
                           small_vector<index_t> &list, std::size_t &min,
                           std::size_t &max) const {
  debugln(true,
          "Quadtree::queryLeaves("
          "box={}, node, nodeBox={}, list, min={}, max={})",
          box, nodeBox, min, max);

  if (!nodeBox.intersects(box))
    return;
  if (node.isLeaf()) {
    if (node.count == 0)
      return;

    debugln(true, "checking {} leaf elements", node.count);
    auto currElementNode = &elementNodes[node.first];
    for (auto i = 0u; i < node.count; i++) {
      auto &element = elements[currElementNode->elementIndex];
      debugln(true, "element {} : {}", element.ent, element.box);
      if (element.box.intersects(box)) {
        list.emplace_back(currElementNode->elementIndex);
        min = std::min(min, element.ent);
        max = std::max(max, element.ent);
      }
      if (auto next = currElementNode->next; next != NULL_INDEX)
        currElementNode = &elementNodes[next];
    }
  } else {
    debugln(true, "going branches");
    const auto [tlBox, trBox, blBox, brBox] = quadrants(nodeBox);
    queryLeaves(box, nodes[node.first + 0], tlBox, list, min, max);
    queryLeaves(box, nodes[node.first + 1], trBox, list, min, max);
    queryLeaves(box, nodes[node.first + 2], blBox, list, min, max);
    queryLeaves(box, nodes[node.first + 3], brBox, list, min, max);
  }
}

small_vector<Quadtree::Element>
Quadtree::queryAll(const BoundingBox &box) const {
  debugln(true, "Quadtree::queryAll(box={})", box);

  small_vector<Element> out;
  auto [indices, min, max] = queryLeaves(box);
  if (indices.size() == 0)
    return {};

  const auto range = max - min + 1;
  std::vector<bool> visited;
  visited.resize(range, false);
  for (const auto index : indices) {
    const auto &element = elements[index];
    const auto visitIndex = element.ent - min;
    if (visited[visitIndex])
      continue;
    out.emplace_back(element);
    visited[visitIndex] = true;
  }

  return out;
}