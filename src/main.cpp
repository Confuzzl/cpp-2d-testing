#include <stdexcept>

import app;

import collision;
import debug;
import glm;

int main() {
  // auto a = collision::Polygon::from({}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}});
  // auto b = collision::Polygon::from({}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}});
  // b.translate({0.5, 0});
  // const auto r = collision::resolve(a, b);
  // println("{} {}", glm::to_string(r.a), glm::to_string(r.b));
  // const auto r2 = collision::resolve(b, a);
  // println("{} {}", glm::to_string(r2.a), glm::to_string(r2.b));

  try {
    MAIN_APP.start();
  } catch (const std::runtime_error &e) {
    println("APP RUNTIME ERROR:\n{}", e.what());
  }
}
