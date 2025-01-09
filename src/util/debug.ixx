module;

#include "util/gl.h"

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

import <deque>;
import <fstream>;
import <sstream>;

export constexpr unsigned long long DEBUG_ANY = -1;
export constexpr auto DEBUG_QUADTREE = 0x0001;
export unsigned long long DEBUG_MASK = 0 | DEBUG_QUADTREE;
export struct DebugOutput {
  static constexpr auto LOG_SIZE = 128u;

  std::deque<std::string> log;
  std::size_t size;

  ~DebugOutput() {
    std::ofstream out;
    out.open("debug.txt");
    std::ostringstream s;
    std::copy(log.rbegin(), log.rend(),
              std::ostream_iterator<std::string>(s, "\n"));
    out << s.str();
    out.close();

    log.~deque();
  }

  void append(std::string &&str) {
    if (size > LOG_SIZE) {
      log.pop_front();
    }
    log.push_front(std::move(str));
    size++;
  }

} DEBUG_OUTPUT;
export template <auto MASK = DEBUG_ANY, typename... Args>
void debugln(const bool print, std::format_string<Args...> str,
             Args &&...args) {
  if (!(DEBUG_MASK & MASK))
    return;
  std::string msg = std::format(str, std::forward<Args>(args)...);
  if (print)
    println(msg);
  DEBUG_OUTPUT.append(std::move(msg));
}