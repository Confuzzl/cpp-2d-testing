module circle;

import polygon;

Circle::Circle(const glm::vec2 &pos, const float rot, const float radius)
    : Collider(pos, rot), radius{radius} {}

Circle Circle::New(const float radius, const glm::vec2 &pos, const float rot) {
  return {pos, rot, radius};
}
