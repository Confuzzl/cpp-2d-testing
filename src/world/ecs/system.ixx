export module system;

import <set>;

export namespace ecs {
using EntID = unsigned int;

namespace sys {
struct System {
  std::set<EntID> entities;
};

struct Manager {};
} // namespace sys
} // namespace ecs