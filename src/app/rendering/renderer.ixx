module;

#include "util/gl.h"

export module rendering;

import glm;

import buffer_object;
import gui_frame;
import world_frame;

import query_object;

export struct Renderer {
  static const glm::mat4 UI_MATRIX;

  query_object_t queryObject;
  GLint elapsed = 0;

  VBOHandle *point;
  VBOHandle *line;
  VBOHandle *tri;
  VBOHandle *quad;

  GUIFrame guiFrame;
  WorldFrame worldFrame;

  Renderer();

  void init();

  void renderFrame(const double t);
};