module;

#include "util/gl.h"
#include "util/main_objects.h"
#include <algorithm>

module input_handler;

import app;
import camera;
import debug;
import rendering;

glm::ivec2 InputHandler::cursorPos{0, 0};
std::map<int, Key> InputHandler::keys{
    {GLFW_KEY_ESCAPE, {[](const double) { MAIN_APP.close(); }}},
    {GLFW_KEY_W, {Key::moveFunction({0, +1})}},
    {GLFW_KEY_A, {Key::moveFunction({-1, 0})}},
    {GLFW_KEY_S, {Key::moveFunction({0, -1})}},
    {GLFW_KEY_D, {Key::moveFunction({+1, 0})}},
};

void InputHandler::processInput(const double dt) {
  glfwPollEvents();
  for (auto &[keycode, key] : keys)
    key(dt);
}

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
  if (const auto iterator = keys.find(key); iterator != keys.cend())
    iterator->second.react(action);
}
void InputHandler::mousePosCallback(GLFWwindow *window, double xpos,
                                    double ypos) {
  cursorPos = {std::clamp<double>(xpos, 0, App::WIDTH),
               std::clamp<double>(ypos, 0, App::HEIGHT)};
}
void InputHandler::mouseClickCallback(GLFWwindow *window, int button,
                                      int action, int mods) {}
void InputHandler::scrollCallback(GLFWwindow *window, double xpos,
                                  double ypos) {
  const float nextFraction =
      MAIN_CAMERA.zoomFraction() + 0.1f * static_cast<float>(ypos);
  if (nextFraction <= 0.5f) {
    // MAIN_CAMERA.fraction = 1.0f;
    // MAIN_CAMERA.exponent /= 2.0f;
    MAIN_CAMERA.halfZoom();
  } else if (nextFraction >= 2.0f) {
    // MAIN_CAMERA.fraction = 1.0f;
    // MAIN_CAMERA.exponent *= 2.0f;
    MAIN_CAMERA.doubleZoom();
  } else {
    // MAIN_CAMERA.fraction = nextFraction;
    MAIN_CAMERA.setZoomFraction(nextFraction);
  }
}
