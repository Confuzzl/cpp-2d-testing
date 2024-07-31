module;

#include "util/gl.h"
#include <stdexcept>

export module window;

// should be init first
export struct Window {
  static constexpr GLsizei WIDTH = 1280, HEIGHT = 720;
  static constexpr float ASPECT_RATIO = static_cast<float>(WIDTH) / HEIGHT;

  GLFWwindow *window;

  Window() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "2D Physics Engine", NULL, NULL);
    if (!window) {
      throw std::runtime_error{"window creation error"};
    }
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);
  }

  operator GLFWwindow *() { return window; }
};