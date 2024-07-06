#pragma once

namespace lua {
constexpr auto is_digit(char c) {
  return c >= '0' && c <= '9';
}

constexpr auto is_hex_digit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

constexpr auto is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

constexpr auto is_alphanumeric(char c) {
  return is_alpha(c) || is_digit(c);
}

constexpr auto is_alpha_or_(char c) {
  return is_alpha(c) || c == '_';
}

constexpr auto is_alphanumeric_or_(char c) {
  return is_alphanumeric(c) || c == '_';
}

constexpr auto is_newline(char c) {
  return c == '\n' || c == '\r';
}
}  // namespace lua
