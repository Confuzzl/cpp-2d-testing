export module bvh2;

import <vector>;
import <array>;
import <algorithm>;
import runtime_array;
import <span>;
import aabb;

export namespace collision {
struct BoundingVolumeHierarchy {
  struct Object {
    std::size_t entID;
    BoundingBox box;
  };
  using T = BoundingBox;

  enum CONSTRUCTION_SCHEME { TOP_DOWN, BOTTOM_UP, INCREMENTAL };
  static constexpr CONSTRUCTION_SCHEME SCHEME = TOP_DOWN;
  static constexpr std::size_t MAX_OBJECTS_PER_LEAF = 2;

  struct Node {
    Node *parent = nullptr;
    BoundingBox box;

    union {
      std::vector<T *> array{};
      struct {
        std::unique_ptr<Node> left, right;
      };
    };
    bool _isLeaf = false;
    unsigned int depth = 0;

    Node() = default;
    ~Node();
    Node(const Node &) = delete;
    Node(Node &&that)
        : parent{that.parent}, box{that.box}, _isLeaf{that._isLeaf},
          depth{that.depth} {
      if (_isLeaf)
        array = std::move(that.array);
      else {
        left = std::move(that.left);
        right = std::move(that.right);
      }
    }
    Node &operator=(const Node &) = delete;
    Node &operator=(Node &&that) {
      parent = std::move(that.parent);
      box = std::move(that.box);
      _isLeaf = std::move(that._isLeaf);
      depth = std::move(that.depth);
      if (_isLeaf)
        array = std::move(that.array);
      else {
        left = std::move(that.left);
        right = std::move(that.right);
      }
      return *this;
    }

    bool isRoot() const;
    bool isLeaf() const;
    bool isBranch() const;

    void setArray(std::span<T *> objects);
    void setChildren(std::unique_ptr<Node> &&left,
                     std::unique_ptr<Node> &&right);
  } root;

  bool query(const BoundingBox &box) const;
  std::vector<T *> queryAll(const BoundingBox &box);

  void add(const T *const o);
  void add(const std::vector<T *> &list) {
    for (const T *const o : list)
      add(o);
  }

  void remove(const T &h);
  template <typename L> void remove(const L &list) {
    for (const T &h : list)
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

  template <CONSTRUCTION_SCHEME>
  static BoundingVolumeHierarchy specializeFrom(const std::vector<T *> &list);
  template <>
  static BoundingVolumeHierarchy
  specializeFrom<TOP_DOWN>(const std::vector<T *> &list) {
    return topDown(list);
  }
  template <>
  static BoundingVolumeHierarchy
  specializeFrom<BOTTOM_UP>(const std::vector<T *> &list) {
    return bottomUp(list);
  }
  template <>
  static BoundingVolumeHierarchy
  specializeFrom<INCREMENTAL>(const std::vector<T *> &list) {
    return incremental(list);
  }

public:
  static BoundingVolumeHierarchy from(const std::vector<T *> &list) {
    return specializeFrom<SCHEME>(list);
  }
};
} // namespace collision