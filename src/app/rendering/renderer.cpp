module;

#include "util/gl.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>

module rendering;

import <iostream>;
import <format>;
import vector;

import glm;
import debug;
import vertices;
// import buffer_objects;
import mesh;
import app;

import shaders;
import texture;

const glm::mat4 Renderer::UI_MATRIX{
    glm::ortho(0.0f, static_cast<float>(App::WIDTH), 0.0f,
               static_cast<float>(App::HEIGHT))};

Renderer::Renderer() {}
void Renderer::init() {
  try {
    shaders::init();
    tex::init();
  } catch (const std::runtime_error &e) {
    println("{}", e.what());
    MAIN_APP.close();
  }
  queryObject.init();
}

void Renderer::renderFrame(const double t) {
  glBeginQuery(GL_TIME_ELAPSED, queryObject.ID);
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // worldFrame.render();
  guiFrame.render();

  glfwSwapBuffers(MAIN_APP.window);
  glEndQuery(GL_TIME_ELAPSED);
  glGetQueryObjectiv(queryObject.ID, GL_QUERY_RESULT, &elapsed);
}