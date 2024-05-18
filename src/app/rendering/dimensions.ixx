export module dimensions;

import glm;

export struct Dimensions {
  glm::vec2 min, max;

  static Dimensions sized(const glm::vec2 &start, const glm::vec2 &size) {
    return {start, start + size};
  }
  static Dimensions fromTo(const glm::vec2 &from, const glm::vec2 &to) {
    return {from, to};
  }
  static Dimensions centerRadius(const glm::vec2 &center, const float radius) {
    return {center - glm::vec2{radius}, center + glm::vec2{radius}};
  }
};