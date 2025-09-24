#pragma once

#include <cstddef>
#include <cstring>
#include <exception>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <span>

namespace utils {
using buffer_t = std::vector<std::byte>;
using rospan_t = std::span<const std::byte>;
using string_t = std::string;

// String / Buffer conversion
inline buffer_t to_bytes(const string_t &str) {
  buffer_t bytes{};
  bytes.reserve(str.size());
  std::memcpy(bytes.data(), str.data(), str.size());
  return bytes;
}

inline string_t to_string(buffer_t const &bytes) {
  string_t str{};
  str.reserve(bytes.size());
  std::memcpy(str.data(), bytes.data(), bytes.size());
  return str;
}

// Print Helpers
inline std::unique_ptr<std::istream> instream =
    std::make_unique<std::istream>(std::cin.rdbuf());
inline std::unique_ptr<std::ostream> outstream =
    std::make_unique<std::ostream>(std::cout.rdbuf());

template <typename... Args>
inline void Print(std::format_string<Args...> fmt, Args &&...args) {
  *outstream << std::format(fmt, std::forward<Args>(args)...);
}
template <typename... Args>
inline void PrintLn(std::format_string<Args...> fmt, Args &&...args) {
  *outstream << std::format(fmt, std::forward<Args>(args)...) << std::endl;
}

inline void PrintLn(std::string_view msg) { PrintLn("{}", msg); }

// Print utils
inline void PrintHexArray(const rospan_t& buffer) {
  for (auto i : buffer) {
    *outstream << std::format("{:02X}", std::to_integer<int>(i));
  }
  *outstream << '\n';
}
inline void PrintHexArray(const std::byte* data, size_t size) {
  PrintHexArray(rospan_t(data, size));
}
inline void PrintError(const std::exception& e) {
  *outstream << std::format("Error: {}", e.what()) << std::endl;
}

// Time measurement helpers
using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
inline time_point_t now() {
  return std::chrono::high_resolution_clock::now();
}
inline void PrintElapsed(const time_point_t& start, const time_point_t& end, std::string_view msg = "elapsed"
  ,size_t processed_size = 0) {
  auto const duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  PrintLn("{} time [ms]: {}", msg, duration);
  if (processed_size > 0) {
    PrintLn("{} throughput [MB/sec]: {}", msg, (double)processed_size / ((double)duration / 1000.0) / (1024.0 * 1024.0));
  }
}

} // namespace utils
