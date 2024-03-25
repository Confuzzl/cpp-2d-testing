module;

#include <utility>

export module update_cycle;

export struct UpdateCycle {
  unsigned short rate;
  double length;
  double prevTime = 0, currTime = 0, dt = 0;
  unsigned long long prevCount = 0, currCount = 0;

  UpdateCycle(const unsigned short rate) : rate{rate}, length{1.0 / rate} {};

  void bottleNeck(const unsigned short bottleneck) {
    const unsigned short r{std::min(rate, bottleneck)};
    rate = r;
    length = 1.0 / rate;
  }

  bool pastLength(const double time) { return time - currTime >= length; }
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