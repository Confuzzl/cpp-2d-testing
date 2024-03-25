module;

#include "util/gl.h"

export module input_handler;

import key;

import glm;

import <map>;
import <functional>;

export struct InputHandler {
  static glm::ivec2 cursorPos;
  static std::map<int, Key> keys;

  static void processInput(const double dt);

  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void mousePosCallback(GLFWwindow *window, double xpos, double ypos);
  static void mouseClickCallback(GLFWwindow *window, int button, int action,
                                 int mods);
  static void scrollCallback(GLFWwindow *window, double xpos, double ypos);
};