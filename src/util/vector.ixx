export module vector;

export import <vector>;

export namespace vec {
template <typename T, typename... Args> std::vector<T> New(Args &&...args) {
  std::vector<T> out;
  out.reserve(sizeof...(Args));
  (out.emplace_back(std::forward<Args>(args)), ...);
  return out;
}
} // namespace vec