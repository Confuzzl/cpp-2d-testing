export module sentinel;

export template <typename T, auto NULL> struct Sentinel {
  static constexpr T NULL_VALUE = NULL;

  T val;

  template <typename U>
  constexpr Sentinel(const U v) : val{static_cast<T>(v)} {}

  constexpr operator bool() const { return val == NULL_VALUE; }
  constexpr operator T() const { return val; }
};