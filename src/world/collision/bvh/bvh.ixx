export module bvh;

import <vector>;
import aabb;

export namespace bvh {
struct tree_t {
  struct node_t {};

  std::vector<aabb_t> objs;
  std::vector<node_t> nodes{};

  tree_t(std::vector<aabb_t> &o) : objs{std::move(o)} {
    nodes.resize(2 * o.size() + 1);
  }
};
}; // namespace bvh