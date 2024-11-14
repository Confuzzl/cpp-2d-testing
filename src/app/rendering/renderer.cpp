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

void Renderer::renderFrame(const double t) {
  glBeginQuery(GL_TIME_ELAPSED, queryObject.ID);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  worldFrame.render();
  guiFrame.render();

  glfwSwapBuffers(MAIN_APP.window);
  glEndQuery(GL_TIME_ELAPSED);
  glGetQueryObjectiv(queryObject.ID, GL_QUERY_RESULT, &elapsed);
  minElapsed = std::min(minElapsed, elapsed);
  maxElapsed = std::max(maxElapsed, elapsed);
  elapsedCounter++;
  elapsedAccumulate += elapsed;
}