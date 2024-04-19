module;

#include <ranges>

export module polygon;

import collider;
import glm;
import poly;
import vector;

import <functional>;

export struct Circle;

export struct Polygon : public Collider {
  using vertex_t = glm::vec2;

private:
  std::vector<vertex_t> vertices;

public:
  Polygon(const glm::vec2 &pos, const float rot,
          std::vector<glm::vec2> &&points);

  struct opts_t {
    unsigned char n;
    float r = 1;
    float offset = 0;
  };
  static Polygon New(const opts_t &opts, const glm::vec2 pos = {0, 0},
                     const float rot = 0);

  static Polygon quadFromTo(const opts_t &opts, const glm::vec2 from,
                            const glm::vec2 to);
  static Polygon quadSize(const opts_t &opts, const glm::vec2 pos,
                          const glm::vec2 size);
};
