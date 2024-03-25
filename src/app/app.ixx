module;

#include "util/gl.h"

export module app;

import <memory>;

import update_cycle;
import scene;
import rendering;

export struct App {
  static constexpr GLsizei WIDTH = 1280, HEIGHT = 720;
  static constexpr float ASPECT_RATIO = static_cast<float>(WIDTH) / HEIGHT;

  GLFWwindow *window;

  UpdateCycle loopCycle;
  UpdateCycle updateCycle;
  UpdateCycle frameCycle;
  unsigned int seconds = 0;

  Scene scene{};

  App();
  ~App();

  void start();
  void startUpdate(const double t);
  void processInput(const double dt);
  void startFrame(const double t);

  void close();
} MAIN_APP{};

export Scene &MAIN_SCENE = MAIN_APP.scene;
export Camera &MAIN_CAMERA = MAIN_SCENE.camera;