export module entity;

import <bitset>;
import components;

static unsigned int counter = 0;
export namespace ecs {
template <typename T> auto getID() {
  static auto id = counter++;
  return id;
}
namespace ent {
// struct Entity {
//   std::size_t ID;
//   std::bitset<comp::MAX_COMPONENTS> signature;
//
//   auto operator<=>(const Entity &that) const { return ID <=> that.ID; }
//
//   template <typename T> void addComponent() { signature.set(getID<T>()); }
// };
} // namespace ent
} // namespace ecs