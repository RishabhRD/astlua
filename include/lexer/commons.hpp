#pragma once

#include <algorithm>
#include <cctype>
#include <iterator>
#include <optional>

namespace lua {
namespace lexer {
template <std::input_iterator Iter>
constexpr auto parse_prefix(Iter begin, Iter end,
                            std::string_view prefix) -> std::optional<Iter> {
  auto [mid_1, mid_2] =
      std::mismatch(begin, end, std::begin(prefix), std::end(prefix));
  if (mid_2 != std::end(prefix))
    return std::nullopt;
  return mid_1;
}

template <std::forward_iterator Iter, std::input_iterator CIter>
constexpr auto find_prefix_in_container(Iter begin, Iter end,
                                        CIter container_begin,
                                        CIter container_end) {
  return std::find_if(
      container_begin, container_end, [begin, end](auto const& token) {
        return parse_prefix(begin, end, token.second) != std::nullopt;
      });
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
