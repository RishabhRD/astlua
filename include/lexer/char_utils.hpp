#pragma once

namespace astlua {
constexpr auto is_digit(char c) { return c >= '0' && c <= '9'; }

constexpr auto is_hex_digit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}
} // namespace astlua
