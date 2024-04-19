module polygon;

import func;

Polygon::Polygon(const glm::vec2 &pos, const float rot,
                 std::vector<glm::vec2> &&points)
    : Collider(pos, rot), vertices{std::move(points)} {}

Polygon Polygon::New(const opts_t &opts, const glm::vec2 pos, const float rot) {
  return {pos, rot, ngonVertices(opts.n, opts.r, opts.offset)};
}
Polygon Polygon::quadFromTo(const opts_t &opts, const glm::vec2 from,
                            const glm::vec2 to) {
  return quadSize(opts, from, to - from);
}
Polygon Polygon::quadSize(const opts_t &opts, const glm::vec2 pos,
                          const glm::vec2 size) {
  const glm::vec2 halfSize = size / 2.0f;
  return Polygon{pos + halfSize, 0,
                 vec::New<glm::vec2>(halfSize, halfSize * glm::vec2{-1, 0},
                                     halfSize * glm::vec2{-1, -1},
                                     halfSize * glm::vec2{0, -1})};
}