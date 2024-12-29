export module small_vector;

import <cstdlib>;
import <vector>;
import <algorithm>;
import <memory>;

// https://stackoverflow.com/questions/41946007
// https://pastebin.com/swvwTgnd
export template <typename T> struct small_vector {
private:
  struct Storage {
    static constexpr auto CAP = 128;

    T buffer[CAP]{};
    T *data = buffer;

    std::size_t count = 0;
    std::size_t cap = CAP;

    bool usingBuffer() const { return data == buffer; }

    void setData(const Storage &that) {
      if (that.usingBuffer()) {
        data = buffer;
      } else {
        reserve(that.count);
      }
    }

    Storage() = default;
    ~Storage() {
      if (!usingBuffer())
        delete[] data;
    }
    Storage(const Storage &that) {
      setData(that);
      count = that.count;
      std::copy(that.data, that.data + that.count, data);
    }
    Storage &operator=(const Storage &that) {
      setData(that);
      count = that.count;
      std::copy(that.data, that.data + that.count, data);
      return *this;
    }
    Storage(Storage &&that) noexcept {
      setData(std::move(that));
      count = that.count;
      std::move(that.data, that.data + that.count, data);
    }
    Storage &operator=(Storage &&that) noexcept {
      setData(std::move(that));
      count = that.count;
      std::move(that.data, that.data + that.count, data);
      return *this;
    }

    void reserve(const std::size_t size) {
      // always true if using buffer
      if (size <= cap)
        return;

      T *newData = new T[size];
      std::move(data, data + count, newData);

      if (!usingBuffer())
        delete[] data;

      data = newData;
      cap = size;
    }
  } storage;

public:
  template <typename... Args> void emplace_back(Args &&...args) {
    if (storage.count >= storage.cap)
      storage.reserve(storage.cap * 2);
    storage.data[storage.count++] = T(std::forward<Args>(args)...);
  }
  void push_back(const T &e) { emplace_back(e); }
  void push_back(T &&e) { emplace_back(std::move(e)); }

  void pop() { storage.count--; }
  void clear() { storage.count = 0; }

  T &operator[](const std::size_t index) { return storage.data[index]; }
  const T &operator[](const std::size_t index) const {
    return storage.data[index];
  }
  T &front() { return storage.data[0]; }
  const T &front() const { return front(); }
  T &back() { return storage.data[storage.count - 1]; }
  const T &back() const { return back(); }

  T *data() { return storage.data; }
  const T *data() const { return data(); }
  const T *cdata() { return data(); }

  T *begin() { return data(); }
  const T *begin() const { return begin(); }
  const T *cbegin() const { return begin(); }
  T *end() { return data() + storage.count; }
  const T *end() const { return end(); }
  const T *cend() const { return end(); }

  std::size_t size() const { return storage.count; }
  bool empty() const { return size() == 0; }

  bool operator=(const small_vector &that) const {
    return std::equal(begin(), end(), that.begin(), that.end());
  }
  bool operator!=(const small_vector &that) const { return !operator=(that); }
};