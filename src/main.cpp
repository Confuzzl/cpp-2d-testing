import debug;
import app;

import glm;

int main() {
  MAIN_APP.start();

  // constexpr float width = 1280, height = 720;
  // constexpr float ratio = width / height;
  // const glm::mat4 view = glm::ortho<float>(-ratio, +ratio, -1, +1);
  // const glm::mat4 ortho = glm::ortho<float>(0, width, 0, height);

  // const glm::vec2 ortho_offset = {1 / ortho[0][0], 1 / ortho[1][1]};

  // println(glm::to_string(ortho_offset));

  // const glm::vec2 test = {0, 0};
  // const glm::vec2 offset = {1, 1};

  // println(glm::to_string(view * glm::vec4{offset, 0, 1}));
  // println(
  //     glm::to_string(ortho * glm::vec4{ortho_offset + glm::vec2{1, 1}, 0,
  //     1}));
  //    println(glm::to_string(
  //        ortho * glm::vec4{glm::vec2{width / 2, height / 2} + offset, 0,
  //        1}));
  //    println(glm::to_string(glm::inverse(view) * ortho * glm::vec4{offset, 0,
  //    1}));
}
