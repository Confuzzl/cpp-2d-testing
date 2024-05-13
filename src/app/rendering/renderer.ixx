module;

#include "util/gl.h"

export module rendering;

import glm;

import gui_frame;
import world_frame;

import query_object;

export struct Renderer {
  static const glm::mat4 UI_MATRIX;

  query_object_t queryObject;
  GLint elapsed = 0;

  gui::frame guiFrame{};
  world::frame worldFrame{};

  Renderer();
  /*call after glfwinit*/
  void init();

  void renderFrame(const double t);
} MAIN_RENDERER;