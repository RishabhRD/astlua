#pragma once

#include <iterator>
#include <ranges>
#include "lexer/commons.hpp"
#include "lexer/enum_rep.hpp"

namespace lua {
namespace lexer {

// Postcondition:
//   - parses first matching symbol in ordered_symbol_string_rep
template <std::forward_iterator Iter>
constexpr auto parse_symbol(Iter begin, Iter end) -> std::optional<Iter> {
  auto symbols = std::views::transform(
      ordered_symbol_string_rep, [](auto const& rep) { return rep.second; });
  return parse_contains(begin, end, std::begin(symbols), std::end(symbols));
}

}  // namespace lexer
}  // namespace lua
