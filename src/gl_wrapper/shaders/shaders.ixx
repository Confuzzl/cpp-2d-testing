export module shaders;

export import :shader;
export import :vertex;
export import :fragment;
export import :geometry;
export import :program;

export namespace shaders {
font_t font{};
basic_t basic{};
trans_t shape{};
circle_t circle{};
striped_t striped{};

void init() {
  font.init();
  basic.init();
  shape.init();
  circle.init();
  striped.init();
}
} // namespace shaders