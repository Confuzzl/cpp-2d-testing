export module dimensions;

import glm;

export struct dimension_t {
  glm::vec2 min, max;

  static dimension_t sized(const glm::vec2 &start, const glm::vec2 &size) {
    return {start, start + size};
  }
  static dimension_t fromTo(const glm::vec2 &from, const glm::vec2 &to) {
    return {from, to};
  }
};