export module bvh2;

import <vector>;
import <list>;
// import <optional>;
import <array>;
import aabb;

export namespace collision {
template <has_aabb T> struct BoundingVolumeHierarchy {
  enum CONSTRUCTION_SCHEME { TOP_DOWN, BOTTOM_UP, INCREMENTAL };
  static constexpr CONSTRUCTION_SCHEME SCHEME = INCREMENTAL;
  static constexpr std::size_t MAX_OBJECTS_PER_LEAF = 2;

  struct Node {
    Node *parent = nullptr;
    BoundingBox box;
    union {
      struct {
        Node *left = nullptr, *right = nullptr;
      } children;
      std::array<T *, MAX_OBJECTS_PER_LEAF> boxes;
    };
    bool isLeaf = false;

    bool isRoot() const { return !parent; }
  };
  Node *root = nullptr;

  struct Handle {
    Node *node;
    T *obj;

    T &operator->() { return *obj; }
    const T &operator->() const { return *obj; }

    operator bool() const { return obj; }
  };

  BoundingVolumeHierarchy() = default;
  BoundingVolumeHierarchy(Node *root);
  ~BoundingVolumeHierarchy();

  Handle query(const BoundingBox &b) const;
  std::vector<Handle> queryAll(const BoundingBox &b) const;

  void add(const T &o);
  template <typename L> void add(const L &list) {
    for (const T &o : list)
      add(o);
  }

  void remove(const Handle &h);
  template <typename L> void remove(const L &list) {
    for (const Handle &h : list)
      remove(h);
  }

  template <typename L> static Node *topDown(const L &list) { return nullptr; }
  template <typename L> static Node *bottomUp(const L &list) { return nullptr; }
  template <typename L> static Node *incremental(const L &list) {
    return nullptr;
  }

  template <typename L> static BoundingVolumeHierarchy from(const L &list) {
    static constexpr auto sswitch = []() constexpr {
      switch (BoundingVolumeHierarchy::SCHEME) {
      case TOP_DOWN:
        return &BoundingVolumeHierarchy::topDown<L>;
      case BOTTOM_UP:
        return &BoundingVolumeHierarchy::bottomUp<L>;
      case INCREMENTAL:
        return &BoundingVolumeHierarchy::incremental<L>;
      }
      return nullptr;
    };

    return {sswitch()(list)};
  }

  // std::optional<Handle> query(const BoundingBox& b);
  // std::vector<Handle> queryAll(const BoundingBox& b);

  // void remove(const Handle& b);
};
} // namespace collision