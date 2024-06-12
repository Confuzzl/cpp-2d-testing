module;

#include "util/gl.h"

export module rendering;

import glm;

import buffer_object;
import gui_frame;
import world_frame;
import vertices;

import query_object;

export struct Renderer {
  static const glm::mat4 UI_MATRIX;

  query_object_t queryObject;
  GLint elapsed = 0;

  VBOHolder vboHolder{};
  EBOHolder eboHolder{};

  VBOHandle point = vboHolder.get<vertex::simple>(1);
  VBOHandle line = vboHolder.get<vertex::simple>(2);
  VBOHandle tri = vboHolder.get<vertex::simple>(3);
  VBOHandle quad = vboHolder.get<vertex::simple>(4);

  GUIFrame guiFrame{};
  WorldFrame worldFrame{};

  Renderer();

  void init();

  void renderFrame(const double t);
};