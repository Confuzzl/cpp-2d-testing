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

  // GLFWwindow *window;

  // Must initialize first in order to load GLFW and glad
  Window window;

  UpdateCycle loopCycle;
  UpdateCycle updateCycle;
  UpdateCycle frameCycle;
  unsigned int seconds = 0;

  Renderer renderer{};

  Scene scene{};

  App();
  ~App();

  void start();
  void startUpdate(const double t);
  void processInput(const double dt);
  void startFrame(const double t);

  void close();
} MAIN_APP{};

// export Scene &MAIN_SCENE = MAIN_APP.scene;
//
// export struct Camera;
// export Camera &MAIN_CAMERA = MAIN_SCENE.camera;
//
// export Renderer &MAIN_RENDERER = MAIN_APP.renderer;
//
// export struct VBOHolder;
// export VBOHolder &VBO_HOLDER = MAIN_RENDERER.vboHolder;
//
// export struct EBOHolder;
// export EBOHolder &EBO_HOLDER = MAIN_RENDERER.eboHolder;
//
// export struct VBOHandle;
// export VBOHandle &POINT = MAIN_RENDERER.point;
// export VBOHandle &LINE = MAIN_RENDERER.line;
// export VBOHandle &TRI = MAIN_RENDERER.tri;
// export VBOHandle &QUAD = MAIN_RENDERER.quad;