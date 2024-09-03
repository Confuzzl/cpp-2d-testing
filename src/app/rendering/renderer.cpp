module;

#include "util/gl.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>

module rendering;

import <iostream>;
import <format>;
import <vector>;

import glm;
import debug;
import app;

const glm::mat4 Renderer::UI_MATRIX{
    glm::ortho<float>(0, App::WIDTH, 0, App::HEIGHT)};

void Renderer::renderFrame(const double t) {
  glBeginQuery(GL_TIME_ELAPSED, queryObject.ID);
  glClearColor(0.9f, 0.9f, 0.9f, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  worldFrame.render();
  guiFrame.render();

  glfwSwapBuffers(MAIN_APP.window);
  glEndQuery(GL_TIME_ELAPSED);
  glGetQueryObjectiv(queryObject.ID, GL_QUERY_RESULT, &elapsed);
}