export module glm;

export import <glm/glm.hpp>;
export import <glm/gtc/matrix_transform.hpp>;
export import <glm/gtc/type_ptr.hpp>;
export import <glm/gtx/string_cast.hpp>;
export import <glm/gtx/matrix_interpolation.hpp>;
export import <glm/gtx/norm.hpp>;

export constexpr glm::vec2 cw_perp(const glm::vec2 v) { return {v.y, -v.x}; }
export constexpr glm::vec2 ccw_perp(const glm::vec2 v) { return {-v.y, v.x}; }

export glm::vec2 transform(const glm::vec2 p, const glm::vec2 v,
                           const float r) {
  const float sin = std::sin(r), cos = std::cos(r);
  return glm::vec2{p.x * cos - p.y * sin, p.x * sin + p.y * cos} + v;
}

import math;
export glm::vec2 random_vec(const glm::vec2 min, const glm::vec2 max) {
  return {random_float(min.x, max.x), random_float(min.y, max.y)};
}

import <format>;
export std::string vec_string(const glm::vec2 vec) {
  return std::format("({:+.3f} {:+.3f})", vec.x, vec.y);
}

export namespace glm {
template <typename T>
concept has_value_ptr = requires(T t) {
  { glm::value_ptr(t) } -> std::convertible_to<void *>;
};

constexpr glm::mat4 ortho(const float x, const float y) {
  return {{2.0f / x, 0.0f, 0.0f, 0.0f},
          {0.0f, 2.0f / y, 0.0f, 0.0f},
          {0.0f, 0.0f, -1.0f, 0.0f},
          {-1.0f, -1.0f, 0.0f, 1.0f}};
}
constexpr glm::mat4 ortho(const glm::vec2 v) { return ortho(v.x, v.y); }
} // namespace glm