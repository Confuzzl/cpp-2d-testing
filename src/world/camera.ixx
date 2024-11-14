export module camera;

import glm;

export struct Camera {
private:
  glm::vec2 pos;
  float speed = 2.0f;

  float fraction = 1.0f;
  float exponent = 1.0f;

  glm::mat4 proj;

public:
  Camera(const glm::vec2 pos = {} /*, const float zoom = 1*/);

  glm::vec2 getPos() const;
  void translate(const glm::vec2 v);

  glm::mat4 getView() const;
  const glm::mat4 &getProj() const;

  float zoomFraction() const { return fraction; }
  void setZoomFraction(const float f) { fraction = f; }
  float zoomExponent() const { return exponent; }
  void halfZoom() {
    exponent /= 2.0f;
    setZoomFraction(1.0f);
  }
  void doubleZoom() {
    exponent *= 2.0f;
    setZoomFraction(1.0f);
  }
  float zoom() const { return fraction * exponent; }

  float relSpeed() const;
};