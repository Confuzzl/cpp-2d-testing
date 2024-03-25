module;

#include "util/gl.h"

module key;

import app;
import debug;

Key::Key(const Callback &processJustOn, const Callback &processOn,
         const Callback &processJustOff, const Callback &processOff)
    : processJustOn{processJustOn}, processOn{processOn},
      processJustOff{processJustOff}, processOff{processOff} {}
Key::Key(const Callback &processOn, const Callback &processOff)
    : Key(processOn, processOn, processOff, processOff){};
Key::Key(const Callback &processOn) : Key(processOn, NONE()) {}

void Key::change(int action) {
  switch (action) {
  case GLFW_RELEASE: {
    on = false;
    justUsed = true;
    break;
  }
  case GLFW_PRESS: {
    on = true;
    justUsed = true;
    break;
  }
  case GLFW_REPEAT: {
  }
  }
}

void Key::operator()(const double dt) {
  if (on) {
    if (justUsed) {
      processJustOn(dt);
      justUsed = false;
    } else {
      processOn(dt);
    }
  } else {
    if (justUsed) {
      processJustOff(dt);
      justUsed = false;
    } else {
      processOff(dt);
    }
  }
}

Key::Callback Key::moveFunction(const glm::vec2 &direction) {
  return [direction](const double dt) {
    MAIN_CAMERA.translate(direction *
                          static_cast<float>(dt * MAIN_CAMERA.relSpeed()));
  };
}

Key::Callback Key::NONE() {
  static Callback out{[](const double) {}};
  return out;
}
