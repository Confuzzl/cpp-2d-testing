export module hash_grid;

import <map>;
import <unordered_map>;
import <vector>;
import glm;
import aabb;
import debug;

export namespace collision {
struct SpatialHashGrid {
  static constexpr auto HASH = std::hash<int>{};
  static constexpr auto CELL_SIZE = 4u;
  static constexpr auto UNIVERSE_RADIUS = 4;

  /*
   0 | 4 | 8 | c
  ---+---+---+---
   1 | 5 | 9 | d
  ---+---+---+---
   2 | 6 | a | e
  ---+---+---+---
   3 | 7 | b | f
  */

  struct EntBox {
    std::size_t entID;
    BoundingBox box;
  };

  std::map<std::size_t, std::map<std::size_t, BoundingBox>> table;

  glm::ivec2 cellCoordinate(const glm::vec2 p) {
    return {std::floor(p.x / CELL_SIZE), std::floor(p.y / CELL_SIZE)};
  }

  void insert(const std::size_t ent, const BoundingBox &box) {
    const auto min = cellCoordinate(box.min), max = cellCoordinate(box.max);

    println("[{}: {}], [{} - {},{} - {}]", ent, box, min.x, max.x, min.y,
            max.y);

    for (int x = min.x; x <= max.x; x++) {
      for (int y = min.y; y < max.y; y++) {
        table[HASH(x) ^ HASH(y)].emplace(ent, box);
      }
    }
  }
  void remove(const std::size_t ent, const BoundingBox &box) {
    const auto min = cellCoordinate(box.min), max = cellCoordinate(box.max);

    for (int x = min.x; x <= max.x; x++) {
      for (int y = min.y; y < max.y; y++) {
        table.at(HASH(x) ^ HASH(y)).erase(ent);
      }
    }
  }

  bool query(const BoundingBox &query) {
    const auto min = cellCoordinate(query.min), max = cellCoordinate(query.max);

    for (int x = min.x; x <= max.x; x++) {
      for (int y = min.y; y < max.y; y++) {
        const auto &list = table.at(HASH(x) ^ HASH(y));
        for (const auto &[entID, box] : list) {
          if (box.intersects(query))
            return true;
        }
      }
    }
    return false;
  }
  std::vector<EntBox> queryAll(const BoundingBox &query) {
    const auto min = cellCoordinate(query.min), max = cellCoordinate(query.max);

    std::vector<EntBox> out;
    out.reserve(10);

    for (int x = min.x; x <= max.x; x++) {
      for (int y = min.y; y < max.y; y++) {
        const auto &list = table.at(HASH(x) ^ HASH(y));
        for (const auto &[entID, box] : list) {
          if (box.intersects(query))
            out.emplace_back(entID, box);
        }
      }
    }
    return out;
  }
};
} // namespace collision