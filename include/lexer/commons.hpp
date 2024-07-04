#pragma once

#include <algorithm>
#include <cctype>
#include <iterator>
#include <optional>

namespace lua {
namespace lexer {
template <std::input_iterator Iter, std::input_iterator PIter>
constexpr auto parse_prefix(Iter begin, Iter end, PIter p_begin,
                            PIter p_end) -> std::optional<Iter> {
  auto [mid_1, mid_2] = std::mismatch(begin, end, p_begin, p_end);
  if (mid_2 != p_end)
    return std::nullopt;
  return mid_1;
}

// Postcondition:
//   - parses if prefix of [begin, end) == prefix
template <std::input_iterator Iter>
constexpr auto parse_prefix(Iter begin, Iter end, std::string_view prefix) {
  return parse_prefix(begin, end, std::begin(prefix), std::end(prefix));
}

template <std::forward_iterator Iter, std::input_iterator CIter>
constexpr auto parse_contains(Iter begin, Iter end, CIter container_begin,
                              CIter container_end) -> std::optional<Iter> {
  auto token_itr = std::find_if(
      container_begin, container_end, [begin, end](auto const& entry) {
        return parse_prefix(begin, end, entry) != std::nullopt;
      });

  if (token_itr == container_end)
    return std::nullopt;

  return parse_prefix(begin, end, *token_itr);
}
}  // namespace lexer
}  // namespace lua
