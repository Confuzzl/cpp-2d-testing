export module collider;

import glm;
import poly;

export struct Polygon;
export struct Circle;

export struct Collider {
protected:
  glm::vec2 position{};
  float rotation = 0;

public:
  Collider(const glm::vec2 &pos, const float rot);

  glm::vec2 pos() const;
  void translate(const glm::vec2 &v);
  void setPos(const glm::vec2 &v);
  float rot() const;
  void rotate(const float r);
  void setRot(const float r);
};
