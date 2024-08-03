#include <stdexcept>

import app;

import debug;

int main() {
  try {
    MAIN_APP.start();
  } catch (const std::runtime_error &e) {
    println(e.what());
  }
}
