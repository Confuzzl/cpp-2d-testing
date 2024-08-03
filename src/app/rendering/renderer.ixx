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
  static const glm::mat4 UI_MATRIX;

  GL::QueryObject queryObject;
  GLint elapsed = 0;

  GUIFrame guiFrame;
  WorldFrame worldFrame;

  struct {
    shaders::TexCol texcol;
    shaders::Sdf sdf;
    shaders::Basic basic;
    shaders::Transform trans;
    shaders::Striped striped;
    shaders::Line line;
    shaders::Circle circ;
  } shaders;

  struct {
    GL::Texture font{"consolas1024.png"};
    GL::Texture sdfFont{"sdf1024.png"};
  } textures;

  struct {
    VBOAllocator vboHeap;
    EBOAllocator eboHeap;
    VBOHandle vbo1 = vboHeap.get<>(1);
    VBOHandle vbo2 = vboHeap.get<>(2);
    VBOHandle vbo3 = vboHeap.get<>(3);
    VBOHandle vbo4 = vboHeap.get<>(4);
  } allocators;

  void renderFrame(const double t);

  Renderer() = default;
};