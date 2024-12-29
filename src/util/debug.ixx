export module debug;

export import <iostream>;
export import <format>;

export template <std::size_t N> struct str {
  char m[N];
  constexpr str(const char (&s)[N]) { std::copy_n(s, N, m); }
};

export void println(const std::string &str = "") { std::cout << str << "\n"; }
export template <typename... Args>
void println(std::format_string<Args...> str, Args &&...args) {
  println(std::format(str, std::forward<Args>(args)...));
}
export template <typename T> void println(const T &s) { println("{}", s); }
