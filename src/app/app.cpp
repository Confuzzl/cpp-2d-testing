module;

#include "util/gl.h"
#include "util/main_objects.h"
#include <stdexcept>

module app;

import input_handler;
import debug;
import gl_debug;
import rendering;

App::App() : loopCycle{0}, updateCycle{120}, frameCycle{60} {
  // glfwInit();

  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // window = glfwCreateWindow(WIDTH, HEIGHT, "2D Physics Engine", NULL, NULL);
  // if (window == NULL) {
  //   throw std::runtime_error{"window creation error"};
  // }
  // glfwMakeContextCurrent(window);

  // gladLoadGL();
  // glViewport(0, 0, WIDTH, HEIGHT);

  glfwSetCursorPosCallback(window, InputHandler::mousePosCallback);
  glfwSetMouseButtonCallback(window, InputHandler::mouseClickCallback);
  glfwSetScrollCallback(window, InputHandler::scrollCallback);
  glfwSetKeyCallback(window, InputHandler::keyCallback);

  glPolygonMode(GL_FRONT, GL_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GL::debugCallback, 0);
}
App::~App() {
  println("app terminated at {:.2f}s", glfwGetTime());
  glfwDestroyWindow(window);
  glfwTerminate();
}

void App::start() {
  MAIN_RENDERER.init();
  MAIN_SCENE.init();

  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();

    loopCycle.pushNewTime(currentTime);
    if (updateCycle.pastLength(currentTime)) {
      startUpdate(currentTime);
    }
    if (frameCycle.pastLength(currentTime)) {
      startFrame(currentTime);
    }
    if (currentTime - seconds >= 1) {
      seconds++;
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