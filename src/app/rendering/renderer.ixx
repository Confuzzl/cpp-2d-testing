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

  QueryObject queryObject;
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
    shaders::circ_t circ;
  } shaders;

  struct {
    tex::texture font{"consolas1024.png"};
    tex::texture sdfFont{"sdf1024.png"};
  } textures;

  struct {
    VBOAllocator vboHeap;
    EBOAllocator eboHeap;
    VBOHandle &VBO_1 = vboHeap.get<1>();
    VBOHandle &VBO_2 = vboHeap.get<2>();
    VBOHandle &VBO_3 = vboHeap.get<3>();
    VBOHandle &VBO_4 = vboHeap.get<4>();
  } allocators;

  void init();

  void renderFrame(const double t);
};