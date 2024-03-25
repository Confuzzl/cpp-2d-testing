export module camera;

import glm;

export struct Camera {
  glm::vec2 pos{};
  float speed = 5;

  float zoom = 1;
  glm::mat4 proj;

  Camera(const glm::vec2 &pos = {}, const float zoom = 1);

  const glm::vec2 &getPos() const;
  void translate(const glm::vec2 &v);

  glm::mat4 getView() const;
  const glm::mat4 &getProj() const;

  float relSpeed() const;
};