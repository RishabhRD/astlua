#pragma once

#include <algorithm>
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
}  // namespace lexer
}  // namespace lua
