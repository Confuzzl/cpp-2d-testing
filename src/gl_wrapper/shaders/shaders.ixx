export module shaders;

export import :shader;
export import :vertex;
export import :fragment;
export import :geometry;
export import :program;

export namespace shaders {
texcol_t texcol{};
sdf_t sdf{};
basic_t basic{};
trans_t trans{};
striped_t striped{};
line_t line{};
circ_t circ{};

void init() {
  texcol.init();
  sdf.init();
  basic.init();
  trans.init();
  striped.init();
  line.init();
  circ.init();
}
} // namespace shaders