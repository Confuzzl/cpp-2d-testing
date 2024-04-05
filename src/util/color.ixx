module;

#include <limits>

export module color;

import debug;

import glm;
import math;

export using color_t = glm::uvec3;
export using rgb_t = glm::u8vec3;
export using rgba_t = glm::u8vec4;

export namespace colors {
const color_t BLACK{0}, DARK_GRAY{63}, GRAY{127}, LIGHT_GRAY{191}, WHITE{255};

const color_t RED{255, 0, 0}, GREEN{0, 255, 0}, BLUE{0, 0, 255};
const color_t MAGENTA{255, 0, 255}, YELLOW{255, 255, 0}, CYAN{0, 255, 255};

color_t random() {
  return {random_int(0u, 255u), random_int(0u, 255u), random_int(0u, 255u)};
}

color_t random_i(const unsigned int i) {
  // https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
  static const auto offset =
      random_int(1u, std::numeric_limits<unsigned int>::max());
  auto num = i + offset;
  num = ((num >> 16) ^ num) * 0x45d9f3b;
  num = ((num >> 16) ^ num) * 0x45d9f3b;
  num = (num >> 16) ^ num;

  return {(0xff'00'00'00 & num) >> 24, (0x00'ff'00'00 & num) >> 16,
          (0x00'00'ff'00 & num) >> 8};
}
} // namespace colors