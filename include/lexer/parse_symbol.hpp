#pragma once

#include <iterator>
#include <ranges>
#include "lexer/commons.hpp"
#include "token.hpp"

namespace lua {
namespace lexer {

template <std::forward_iterator Iter>
constexpr auto parse_symbol(Iter begin, Iter end) -> std::optional<Iter> {
  auto symbols = std::views::transform(
      tokens::symbol_string_rep, [](auto const& rep) { return rep.second; });
  return parse_contains(begin, end, std::begin(symbols), std::end(symbols));
}

}  // namespace lexer
}  // namespace lua
