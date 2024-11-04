export module bezier;

import <vector>;
import aabb;
import glm;

import debug;

std::vector<float> roots(const float a, const float b, const float c) {
  // println("{} {} {}", a, b, c);

  std::vector<float> out;
  out.reserve(2);
  if (a == 0) {
    if (b == 0 || c == 0) {
      // c = 0
      return out;
    } else {
      // bt + c = 0
      out.emplace_back(-c / b);
    }
    return out;
  }

  const float discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    return out;
  const float sqrt = std::sqrt(discriminant);
  const float plus = (-b + sqrt) / (2 * a);
  const float minus = (-b - sqrt) / (2 * a);
  out.emplace_back(plus);
  if (minus != plus)
    out.emplace_back(minus);
  return out;
}

export struct Bezier {
  glm::vec2 a, b, c, d;

  glm::vec2 at(const float t) const {
    const float nt = 1 - t;
    return (nt * nt * nt * a) + (3 * t * nt * nt * b) + (3 * t * t * nt * c) +
           (t * t * t * d);
  }
  BoundingBox box() const {
    std::vector<float> criticalValues;
    criticalValues.reserve(4);
    // (-3a+9b-9c+3d)t^2 + (6a-12b+6c)t + (-3a+3b)
    const auto aCoeff = 3.0f * (d - 3.0f * c + 3.0f * b - a);
    const auto bCoeff = 6.0f * (a - 2.0f * b + c);
    const auto cCoeff = 3.0f * (b - a);

    const std::vector<float> xRoots = roots(aCoeff.x, bCoeff.x, cCoeff.x);
    const std::vector<float> yRoots = roots(aCoeff.y, bCoeff.y, cCoeff.y);

    for (const float t : xRoots) {
      // println(t);
      if (0 < t && t < 1)
        criticalValues.emplace_back(t);
    }

    for (const float t : yRoots) {
      // println(t);
      if (0 < t && t < 1)
        criticalValues.emplace_back(t);
    }
    BoundingBox out;
    out.expand(a);
    out.expand(d);
    for (const float t : criticalValues)
      out.expand(at(t));
    return out;
  }
};

export struct Spline {
  std::vector<Bezier> pieces;

  static Spline begin(const Bezier &b) {
    Spline out;
    out.pieces.emplace_back(b);
    return out;
  }
  Spline add(const glm::vec2 c, const glm::vec2 d) {
    const Bezier &prev = pieces.back();
    pieces.emplace_back(prev.d, 2.0f * prev.d - prev.c, c, d);
    return *this;
  }
  Spline endLoop(const std::size_t index = 0) {
    const Bezier &first = pieces[index];
    return add(2.0f * first.a - first.b, first.a);
  }
};
