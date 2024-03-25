module polygon;

import func;

Polygon::Polygon(const glm::vec2 &pos, const float r,
                 std::vector<glm::vec2> &&points)
    : Collider(pos, r), vertices{std::move(points)} {}

Polygon Polygon::New(const opts_t &opts, const glm::vec2 pos, const float r) {
  return {pos, r, ngonVertices(opts.n, opts.r, opts.offset)};
}