#pragma once

#include <algorithm>
#include <iterator>
#include <optional>
#include "lexer/char_utils.hpp"

namespace lua {
namespace lexer {
// Postcondition:
// - Parses if [begin, end) starts with alphabet or _ till contiguous alphanumeric
//   or _
template <std::input_iterator Iter>
constexpr auto parse_identifier(Iter begin, Iter end) -> std::optional<Iter> {
  if (begin == end || (!is_alpha_or_(*begin)))
    return std::nullopt;
  return std::find_if_not(begin, end, is_alphanumeric_or_);
}
}  // namespace lexer
}  // namespace lua
