export module world_frame;

import frame;
import glm;
import color;

// import vertex_layout;
//  import object;
//   import circle;
//  import <variant>;

export struct WorldFrame : BaseFrame {
  void render() override;

  void drawGrid() const;

  // void drawObject(const Object &object) const {
  //   if (std::holds_alternative<Object::CircMesh>(object.colliderVariant)) {
  //     const Circle &circle =
  //         std::get<Object::CircMesh>(object.colliderVariant).circ;
  //     drawCircle(circle.pos(), circle.radius);
  //   } else {
  //     const auto &[poly, mesh] =
  //         std::get<Object::PolyMesh>(object.colliderVariant);
  //     drawMesh(mesh, poly.pos(), poly.rot());
  //   }
  // }
};