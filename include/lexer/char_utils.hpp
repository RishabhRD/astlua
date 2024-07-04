#pragma once

#include <concepts>
#include <utility>

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

template <std::predicate<char> Pred>
constexpr auto or_(Pred pred) {
  return [pred_ = std::move(pred)](auto c) {
    return pred_(c) || c == '_';
  };
}
}  // namespace lua
