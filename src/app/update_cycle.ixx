module;

#include <utility>

export module update_cycle;

export struct UpdateCycle {
  bool locked = false;
  unsigned int rate;
  double length;
  double prevTime = 0, currTime = 0, dt = 0;

  std::size_t prevCount = 0, currCount = 0;

  UpdateCycle(const unsigned int rate) : rate{rate}, length{1.0 / rate} {};

  bool toggle() { return (locked = !locked); }

  void bottleNeck(const unsigned int bottleneck) {
    const auto r = std::min(rate, bottleneck);
    rate = r;
    length = 1.0 / rate;
  }

  bool isPastLength(const double time) const {
    return time - currTime >= length;
  }
  void pushNewTime(const double newCurrTime) {
    currCount++;
    prevTime = currTime;
    currTime = newCurrTime;
    dt = currTime - prevTime;
  }
  void pushCount() {
    prevCount = currCount;
    currCount = 0;
  }
};