export module world_frame;

import frame;
import glm;
import color;

export struct Mesh;

export struct WorldFrame : BaseFrame {
  void render() override;

  void drawGrid() const;
};