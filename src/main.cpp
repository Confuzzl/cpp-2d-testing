#include <stdexcept>

import app;

import debug;

int main() {
  try {
    MAIN_APP.start();
  } catch (const std::exception &e) {
    println(e.what());
  }
}
