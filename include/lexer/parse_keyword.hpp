#pragma once

#include <cctype>
#include <iterator>
#include <ranges>
#include "lexer/commons.hpp"
#include "token.hpp"

namespace lua {
namespace lexer {

template <std::forward_iterator Iter>
constexpr auto parse_keyword(Iter begin, Iter end) -> std::optional<Iter> {
  auto keywords = std::views::transform(
      tokens::keyword_string_rep, [](auto const& rep) { return rep.second; });
  auto res =
      parse_contains(begin, end, std::begin(keywords), std::end(keywords));
  if (res == std::nullopt)
    return std::nullopt;
  if (*res == end || std::isspace(**res))
    return *res;
  return std::nullopt;
}

}  // namespace lexer
}  // namespace lua
