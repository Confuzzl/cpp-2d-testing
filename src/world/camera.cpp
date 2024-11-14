module camera;

import app;

Camera::Camera(const glm::vec2 pos)
    : pos{pos},
      proj{glm::ortho<float>(-App::ASPECT_RATIO, +App::ASPECT_RATIO, -1, +1)} {}

glm::vec2 Camera::getPos() const { return pos; }
void Camera::translate(const glm::vec2 v) { pos += v; }

glm::mat4 Camera::getView() const {
  glm::mat4 out = proj;
  const float zoom = this->zoom();
  out[0][0] *= zoom;
  out[1][1] *= zoom;
  // out = glm::translate(out, glm::vec3{-pos, 0});
  out[3] = out[3] - out[0] * pos.x - out[1] * pos.y; // prevent translate copy
  return out;
}
const glm::mat4 &Camera::getProj() const { return proj; }

float Camera::relSpeed() const { return speed / zoom(); }
