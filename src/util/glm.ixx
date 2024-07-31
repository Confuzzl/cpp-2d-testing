export module glm;

export import <glm/glm.hpp>;
export import <glm/gtc/matrix_transform.hpp>;
export import <glm/gtc/type_ptr.hpp>;
export import <glm/gtx/string_cast.hpp>;
export import <glm/gtx/matrix_interpolation.hpp>;
export import <glm/gtx/norm.hpp>;

export glm::vec2 cw_perp(const glm::vec2 v) { return {v.y, -v.x}; }
export glm::vec2 ccw_perp(const glm::vec2 v) { return {-v.y, v.x}; }

import math;
export glm::vec2 random_vec(const glm::vec2 min, const glm::vec2 max) {
  return {random_float(min.x, max.x), random_float(min.y, max.y)};
}

import <format>;
export std::string vec_string(const glm::vec2 vec) {
  return std::format("({:+.2f} {:+.2f})", vec.x, vec.y);
}

export namespace glm {
template <typename T>
concept has_value_ptr = requires(T t) {
  { glm::value_ptr(t) } -> std::convertible_to<void *>;
};
}