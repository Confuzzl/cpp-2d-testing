export module rendering;

import glm;

import gui_frame;
import world_frame;

export struct Renderer {
  static const glm::mat4 UI_MATRIX;

  gui::frame guiFrame{};
  world::frame worldFrame{};

  Renderer();
  /*call after glfwinit*/
  void init();

  void renderFrame(const double t) const;
} MAIN_RENDERER;