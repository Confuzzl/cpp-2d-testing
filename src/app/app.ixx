module;

#include "util/gl.h"

export module app;

import <memory>;

import update_cycle;
import scene;
import rendering;
import window;
import glm;
// import aabb;

export struct App {
  static constexpr GLsizei WIDTH = 1280, HEIGHT = 720;
  static constexpr glm::ivec2 DIMENSIONS{WIDTH, HEIGHT};
  static constexpr glm::mat4 UI_MATRIX{{2.0f / WIDTH, 0.0f, 0.0f, 0.0f},
                                       {0.0f, 2.0f / HEIGHT, 0.0f, 0.0f},
                                       {0.0f, 0.0f, -1.0f, 0.0f},
                                       {-1.0f, -1.0f, 0.0f, 1.0f}};
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
