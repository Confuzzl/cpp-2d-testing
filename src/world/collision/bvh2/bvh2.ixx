export module bvh2;

import <vector>;
import <list>;
import <optional>;

export struct BoundingBox;

export namespace collision {
struct BoundingVolumeHierarchy {
  enum CONSTRUCTION_SCHEME { TOP_DOWN, BOTTOM_UP, INCREMENTAL };
  static constexpr CONSTRUCTION_SCHEME SCHEME = INCREMENTAL;

  using List = std::list<BoundingBox*>;
  using Iter = List::iterator;
  struct Handle {
    Iter iter;

    BoundingBox* operator*() { return *iter; }
    const BoundingBox* operator*() const { return *iter; }
    BoundingBox& operator->() { return **iter; }
    const BoundingBox& operator->() const { return **iter; }
  };
  List boxes;

  BoundingVolumeHierarchy() = default;
  BoundingVolumeHierarchy(List&& list);

  std::optional<Handle> query(const BoundingBox& b);
  std::vector<Handle> queryAll(const BoundingBox& b);

  void remove(const Handle& b);
};
}  // namespace collision