module collider;

import debug;

Collider::Collider(const glm::vec2 &pos, const float rot)
    : position{pos}, rotation{rot} {}

glm::vec2 Collider::pos() const { return position; }
void Collider::translate(const glm::vec2 &v) { position += v; }
void Collider::setPos(const glm::vec2 &v) { translate(v - position); }

float Collider::rot() const { return rotation; }
void Collider::rotate(const float r) { setRot(rotation + r); }
void Collider::setRot(const float r) { rotation = r; }
