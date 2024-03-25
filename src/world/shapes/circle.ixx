export module circle;

import collider;
import glm;

export struct Polygon;

export struct Circle : public Collider {
  const float radius = 1;

  Circle(const glm::vec2 &pos, const float r, const float radius);

  static Circle New(const float radius = 1, const glm::vec2 &pos = {0, 0},
                    const float r = 0);
};