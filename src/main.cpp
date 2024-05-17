import debug;
import app;

import glm;

int main() {
  // MAIN_APP.start();

  /*
  gl_Position : [-1, 1]

  ortho * {offset, 0, 1} = clip space fixed
  view * {offset, 0, 1} = clip space persp

  ortho * {offset, 0, 1} = view * {?, 0, 1}
  inverse(view) * ortho * {offset, 0, 1}
  */

  constexpr float width = 900, height = 600;
  constexpr float ratio = width / height;
  const glm::mat4 view = glm::ortho<float>(-ratio, +ratio, -1, +1);
  const glm::mat4 ortho = glm::ortho<float>(0, width, 0, height);

  println(2 / ortho[0][0]);
  println(2 / ortho[1][1]);

  const glm::vec2 test = {0, 0};
  const glm::vec2 offset = {1, 1};

  println(glm::to_string(view * glm::vec4{offset, 0, 1}));
  println(glm::to_string(
      ortho * glm::vec4{glm::vec2{width / 2, height / 2} + offset, 0, 1}));
  // println(glm::to_string(glm::inverse(view) * ortho * glm::vec4{offset, 0,
  // 1}));
}
