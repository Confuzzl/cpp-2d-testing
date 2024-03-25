export module key;

import glm;
import <functional>;

export struct Key {
  using Callback = std::function<void(const double)>;

  bool on = false;
  bool justUsed = false;

  const Callback processJustOn;
  const Callback processOn;
  const Callback processJustOff;
  const Callback processOff;

  Key(const Callback &processJustOn, const Callback &processOn,
      const Callback &processJustOff, const Callback &processOff);
  Key(const Callback &processOn, const Callback &processOff);
  Key(const Callback &processOn);

  void change(int action);

  void operator()(const double dt);

  static Callback moveFunction(const glm::vec2 &direction);

  static Callback NONE();
};
