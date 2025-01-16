export module free_list;

import <vector>;
import <concepts>;
import <utility>;

export template <typename T> struct free_list {
private:
  static constexpr std::size_t NULL_INDEX = -1;

  union Element {
    T element;
    std::size_t next;
  };
  std::vector<Element> data;
  std::size_t first = NULL_INDEX;

public:
  template <typename... Args> std::size_t emplace_back(Args &&...args) {
    if (first == NULL_INDEX) {
      data.emplace_back(Element{.element = T(std::forward<Args>(args)...)});
      return data.size() - 1;
    }
    const std::size_t i = first;
    first = data[first].next;
    data[i].element = T(std::forward<Args>(args)...);
    return i;
  }
  std::size_t push_back(const T &e) { return emplace_back(e); }
  std::size_t push_back(T &&e) { return emplace_back(std::move(e)); }

  void erase(const std::size_t index) {
    data[index].next = first;
    first = index;
  }
  void clear() {
    data.clear();
    first = NULL_INDEX;
  }
  std::size_t range() const { return data.size(); }
  T &operator[](const std::size_t i) { return data[i].element; }
  const T &operator[](const std::size_t i) const { return data[i].element; }
};