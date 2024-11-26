module;

#include "util/gl.h"

export module rendering;

import glm;

import query_object;
import gui_frame;
import world_frame;
import bo_heap;
import shaders;
import texture;

export struct Renderer {
  GL::QueryObject queryObject;
  GLint elapsed = 0;
  double elapsedAccumulate = 0;
  GLint elapsedCounter = 0;
  GLint minElapsed = (1u << 31) - 1, maxElapsed = 1 << 31;

  GUIFrame guiFrame;
  WorldFrame worldFrame;

  struct {
    shaders::TexCol texcol;
    shaders::Sdf sdf;
    shaders::Basic basic;
    shaders::Transform trans;
    shaders::Striped striped;
    shaders::Bezier bezier;
    shaders::Line line;
    shaders::Circle circ;
    shaders::Debug debug;
    shaders::BoxBlur boxblur;
    shaders::Outline outline;
  } shaders;

  struct {
    VBOAllocator vboHeap;
    EBOAllocator eboHeap;
    VBOHandle vbo1 = vboHeap.get<>(1);
    VBOHandle vbo2 = vboHeap.get<>(2);
    VBOHandle vbo3 = vboHeap.get<>(3);
    VBOHandle vbo4 = vboHeap.get<>(4);
  } allocators;

  void renderFrame(const double t);

  Renderer();
};
