module circle;

import polygon;

Circle::Circle(const glm::vec2 &pos, const float r, const float radius)
    : Collider(pos, r), radius{radius} {}

Circle Circle::New(const float radius, const glm::vec2 &pos, const float r) {
  return {pos, r, radius};
}
