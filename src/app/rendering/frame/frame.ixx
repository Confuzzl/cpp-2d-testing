export module frame;

import glm;
import color;
import dimensions;

export namespace frame {
struct base_t {
  void render() const;

  void drawPoint(const glm::vec2 &point, const float size = 10,
                 const color_t &color = colors::WHITE) const;
  void drawPointFixed(const glm::vec2 &point, const float size = 10,
                      const color_t &color = colors::WHITE) const;

  void drawLine(const glm::vec2 &from, const glm::vec2 &to,
                const float size = 0.1,
                const color_t &color = colors::WHITE) const;
  void drawLineFixed(const glm::vec2 &from, const glm::vec2 &to,
                     const color_t &color = colors::WHITE) const;

  void drawArrow(const dimension_t &dimensions,
                 const color_t &color = colors::WHITE) const;

  void drawCircle(const dimension_t &dimensions,
                  const color_t &color = colors::WHITE) const;

  void drawBox(const dimension_t &dimensions, const float lineSize = 5,
               const color_t &color = colors::WHITE) const;
  void drawBoxFixed(const dimension_t &dimensions, const float lineSize = 5,
                    const color_t &color = colors::WHITE) const;
  void drawQuad(const dimension_t &dimensions,
                const color_t &color = colors::WHITE) const;
};
} // namespace frame