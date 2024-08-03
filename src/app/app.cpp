module;

#include "util/gl.h"
#include "util/main_objects.h"
#include <stdexcept>

module app;

import input_handler;
import debug;
import gl_debug;
import rendering;

App::App() try : loopCycle{0}, updateCycle{120}, frameCycle{60} {
  glfwSetCursorPosCallback(window, InputHandler::mousePosCallback);
  glfwSetMouseButtonCallback(window, InputHandler::mouseClickCallback);
  glfwSetScrollCallback(window, InputHandler::scrollCallback);
  glfwSetKeyCallback(window, InputHandler::keyCallback);

  glPolygonMode(GL_FRONT, GL_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(debugCallback, 0);
} catch (const std::runtime_error &e) {
  println(e.what());
}
App::~App() {
  println("app terminated at {:.2f}s", glfwGetTime());
  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::start() {
  MAIN_SCENE.init();

  while (!glfwWindowShouldClose(window)) {
    const double currentTime = glfwGetTime();

    loopCycle.pushNewTime(currentTime);
    if (updateCycle.isPastLength(currentTime))
      startUpdate(currentTime);
    if (frameCycle.isPastLength(currentTime))
      startFrame(currentTime);
    if (currentTime - seconds++ >= 1) {
      loopCycle.pushCount();
      updateCycle.pushCount();
      frameCycle.pushCount();
    }
  }
}

void App::startUpdate(const double t) {
  updateCycle.pushNewTime(t);
  InputHandler::processInput(updateCycle.dt);
}
void App::startFrame(const double t) {
  frameCycle.pushNewTime(t);
  MAIN_RENDERER.renderFrame(t);
}

void App::close() { glfwSetWindowShouldClose(window, true); }