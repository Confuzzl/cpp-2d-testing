export module bvh2;

import <vector>;
import <list>;
// import <optional>;
import <variant>;
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

    struct Children {
      std::unique_ptr<Node> left, right;
    };
    std::variant<Children, std::array<T *, MAX_OBJECTS_PER_LEAF>> data;

    bool isRoot() const { return !parent; }
    bool isLeaf() const { return !std::holds_alternative<Children>(data); }

    ~Node() {}
  };
  std::unique_ptr<Node> root;

  struct Handle {
    Node *node;
    T *obj;

    T &operator->() { return *obj; }
    const T &operator->() const { return *obj; }

    operator bool() const { return obj; }
  };

  BoundingVolumeHierarchy() = default;
  BoundingVolumeHierarchy(Node *root);

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

  template <typename L> static BoundingVolumeHierarchy topDown(const L &list) {
    return {};
  }
  template <typename L> static BoundingVolumeHierarchy bottomUp(const L &list) {
    return {};
  }
  template <typename L>
  static BoundingVolumeHierarchy incremental(const L &list) {
    return {};
  }

  template <typename L> static BoundingVolumeHierarchy from(const L &list) {
    if constexpr (SCHEME == TOP_DOWN)
      return topDown(list);
    if constexpr (SCHEME == BOTTOM_UP)
      return bottomUp(list);
    if constexpr (SCHEME == INCREMENTAL)
      return incremental(list);
    return {};
  }

  // std::optional<Handle> query(const BoundingBox& b);
  // std::vector<Handle> queryAll(const BoundingBox& b);

  // void remove(const Handle& b);
};
} // namespace collision