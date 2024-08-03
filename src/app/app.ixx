module;

#include "util/gl.h"

export module app;

import <memory>;

import update_cycle;
import scene;
import rendering;
import window;

export struct App {
  static constexpr GLsizei WIDTH = 1280, HEIGHT = 720;
  static constexpr float ASPECT_RATIO = static_cast<float>(WIDTH) / HEIGHT;

  // Must initialize first in order to load GLFW and glad
  Window window;

  UpdateCycle loopCycle;
  UpdateCycle updateCycle;
  UpdateCycle frameCycle;
  unsigned int seconds = 0;

  Renderer renderer;

  Scene scene;

  App();
  ~App();

  void start();
  void startUpdate(const double t);
  void processInput(const double dt);
  void startFrame(const double t);

  void close();
} MAIN_APP{};
