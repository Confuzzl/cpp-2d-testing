export module color;

import glm;
import math;

export using color_t = glm::uvec3;

export namespace colors {
const color_t BLACK{0}, DARK_GRAY{63}, GRAY{127}, LIGHT_GRAY{191}, WHITE{255};

const color_t RED{255, 0, 0}, GREEN{0, 255, 0}, BLUE{0, 0, 255};
const color_t MAGENTA{255, 0, 255}, YELLOW{255, 255, 0}, CYAN{0, 255, 255};

color_t random() {
  return {random_int(0u, 255u), random_int(0u, 255u), random_int(0u, 255u)};
}
} // namespace colors