export module world_frame;

import frame;
import glm;
import color;

import vertex_layout;
import object;
import circle;
import <variant>;

export struct WorldFrame : BaseFrame {
  void render() override;

  void drawGrid() const;

  void drawObject(const Object &object) const {
    if (std::holds_alternative<Circle>(object.collider)) {
      const Circle &circle = std::get<Circle>(object.collider);
      drawCircle(circle.pos(), circle.radius);
    } else {
      const Object::poly_mesh &polyMesh =
          std::get<Object::poly_mesh>(object.collider);
      drawMesh(polyMesh.mesh, polyMesh.poly.pos(), polyMesh.poly.rot());
    }
  }
};