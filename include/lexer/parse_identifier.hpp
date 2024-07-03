#pragma once

#include <algorithm>
#include <iterator>
#include <optional>

namespace lua {
namespace lexer {
template <std::input_iterator Iter>
constexpr auto parse_identifier(Iter begin, Iter end) -> std::optional<Iter> {
  auto is_alpha_or_ = [](char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  };
  auto is_alphanumeric_or_ = [is_alpha_or_](char c) {
    return is_alpha_or_(c) || (c >= '0' && c <= '9');
  };

  if (begin == end || (!is_alpha_or_(*begin)))
    return std::nullopt;

  return std::find_if_not(begin, end, is_alphanumeric_or_);
}
}  // namespace lexer
}  // namespace lua
