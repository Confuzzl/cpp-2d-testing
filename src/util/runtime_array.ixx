export module runtime_array;

import <memory>;

template <typename L, typename T> static void write(L &&list, T *dest) {
  std::move(list.begin(), list.end(), dest);
}
template <typename L, typename T> static void write(L &list, T *dest) {
  std::copy(list.begin(), list.end(), dest);
}

export template <typename T> struct runtime_array {
private:
  std::unique_ptr<T[]> array;
  std::size_t _size;

public:
  runtime_array(std::unique_ptr<T[]> &&parent, const std::size_t size)
      : array(std::move(parent)), _size{size} {}
  // runtime_array(const runtime_array)
  runtime_array(runtime_array &&) = default;
  runtime_array &operator=(runtime_array &&) = default;

  T &operator[](const std::size_t i) { return *(array.get() + i); }
  const T &operator[](const std::size_t i) const { return *(array.get() + i); }

  T *begin() { return array.get(); }
  const T *begin() const { return array.get(); }
  const T *cbegin() const { return array.get(); }
  T *end() { return begin() + _size; }
  const T *end() const { return begin() + _size; }
  const T *cend() const { return cbegin() + _size; }

  std::size_t size() const { return _size; }
};
export template <typename T, typename L>
runtime_array<T> make_runtime_array(L &&list) {
  auto size = list.size();
  auto out = std::make_unique<T[]>(size);
  write(std::forward<L>(list), out.get());
  return {std::move(out), size};
}
