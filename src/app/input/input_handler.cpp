module;

#include "util/gl.h"
#include <algorithm>

module input_handler;

import app;
import debug;
// import gui_frame;
import rendering;

glm::ivec2 InputHandler::cursorPos{0, 0};
std::map<int, Key> InputHandler::keys{
    {GLFW_KEY_ESCAPE, {[](const double) { MAIN_APP.close(); }}},
    {GLFW_KEY_W, {Key::moveFunction(glm::vec2{0, +1})}},
    {GLFW_KEY_A, {Key::moveFunction(glm::vec2{-1, 0})}},
    {GLFW_KEY_S, {Key::moveFunction(glm::vec2{0, -1})}},
    {GLFW_KEY_D, {Key::moveFunction(glm::vec2{+1, 0})}},
};

void InputHandler::processInput(const double dt) {
  glfwPollEvents();
  for (auto &[keycode, key] : keys)
    key(dt);
}

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
  const auto iterator = keys.find(key);
  if (iterator == keys.end())
    return;
  iterator->second.change(action);
}
void InputHandler::mousePosCallback(GLFWwindow *window, double xpos,
                                    double ypos) {
  cursorPos = {std::clamp<double>(xpos, 0, App::WIDTH),
               std::clamp<double>(ypos, 0, App::HEIGHT)};
}
void InputHandler::mouseClickCallback(GLFWwindow *window, int button,
                                      int action, int mods) {
  // MAIN_RENDERER.guiFrame.
}
void InputHandler::scrollCallback(GLFWwindow *window, double xpos,
                                  double ypos) {
  static float INCREMENT = 0.1f;
  MAIN_CAMERA.zoom = std::fmax(
      INCREMENT, static_cast<float>(MAIN_CAMERA.zoom + ypos * INCREMENT));
}
