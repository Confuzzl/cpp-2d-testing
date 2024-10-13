export module bvh2;

import <vector>;
import <list>;
import <variant>;
import <array>;
import <algorithm>;
import runtime_array;
import <span>;
import aabb;
import object;

export namespace collision {
struct BoundingVolumeHierarchy {
  // using T = world::BaseObject;
  using T = int;

  enum CONSTRUCTION_SCHEME { TOP_DOWN, BOTTOM_UP, INCREMENTAL };
  static constexpr CONSTRUCTION_SCHEME SCHEME = TOP_DOWN;
  static constexpr std::size_t MAX_OBJECTS_PER_LEAF = 2;

  struct Node {
    Node *parent = nullptr;
    BoundingBox box;

    using Array = std::vector<T *>;
    struct Children {
      std::unique_ptr<Node> left, right;
    };
    union {
      Array array;
      Children children{};
    };
    bool _isLeaf = false;
    unsigned int depth = 0;

    Node() = default;
    ~Node();

    bool isRoot() const;
    bool isLeaf() const;
    bool isBranch() const;

    Array &getArray();
    const Array &getArray() const;
    Children &getChildren();
    const Children &getChildren() const;

    void setArray(std::span<T *> objects);
    void setChildren(std::unique_ptr<Node> &&left,
                     std::unique_ptr<Node> &&right);
  };
  std::unique_ptr<Node> root = std::make_unique<Node>();

  struct Handle {
    Node *node;
    T *obj;

    T &operator->() { return *obj; }
    const T &operator->() const { return *obj; }

    operator bool() const { return obj; }
  };

  BoundingVolumeHierarchy() = default;
  BoundingVolumeHierarchy(std::unique_ptr<Node> &&root)
      : root{std::move(root)} {}

  Handle query(const BoundingBox &b) const;
  std::vector<Handle> queryAll(const BoundingBox &b) const;

  void add(const T *const o) {}
  void add(const std::vector<T *> &list) {
    for (const T *o : list)
      add(o);
  }

  void remove(const Handle &h);
  template <typename L> void remove(const L &list) {
    for (const Handle &h : list)
      remove(h);
  }

private:
  static BoundingVolumeHierarchy topDown(const std::vector<T *> &list);
  static void topDownRecurse(Node *const node, std::span<T *> objects,
                             const int depth);

  static BoundingVolumeHierarchy bottomUp(const std::vector<T *> &list) {
    return {};
  }
  static BoundingVolumeHierarchy incremental(const std::vector<T *> &list) {
    BoundingVolumeHierarchy out;
    out.add(list);
    return out;
  }

public:
  static BoundingVolumeHierarchy from(const std::vector<T *> &list) {
    if constexpr (SCHEME == TOP_DOWN)
      return topDown(list);
    if constexpr (SCHEME == BOTTOM_UP)
      return bottomUp(list);
    if constexpr (SCHEME == INCREMENTAL)
      return incremental(list);
    return {};
  }
};
} // namespace collision