#pragma once

#include <algorithm>
#include <iterator>
#include <ranges>
#include "functional.hpp"
#include "lexer/char_utils.hpp"
#include "token.hpp"

namespace lua {
namespace lexer {

// Postcondition:
//   - parses if alphanumeric including _ prefix is a valid keyword
template <std::forward_iterator Iter>
constexpr auto parse_keyword(Iter begin, Iter end) -> std::optional<Iter> {
  auto keywords = std::views::transform(
      tokens::keyword_string_rep, [](auto const& rep) { return rep.second; });

  auto alpha_end = std::find_if_not(begin, end, or_(lift(is_alphanumeric)));
  auto keyword_itr = std::ranges::find_if(keywords, [=](auto keyword) {
    return std::equal(begin, alpha_end, std::begin(keyword), std::end(keyword));
  });

  if (keyword_itr == std::end(keywords))
    return std::nullopt;

  return alpha_end;
}

}  // namespace lexer
}  // namespace lua
