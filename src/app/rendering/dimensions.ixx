export module dimensions;

import glm;

export struct Dimension {
  glm::vec2 min, max;

  static Dimension sized(const glm::vec2 &start, const glm::vec2 &size) {
    return {start, start + size};
  }
  static Dimension fromTo(const glm::vec2 &from, const glm::vec2 &to) {
    return {from, to};
  }
};