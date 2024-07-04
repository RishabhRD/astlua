#pragma once

#include <algorithm>
#include <iterator>
#include <ranges>
#include "lexer/char_utils.hpp"
#include "token.hpp"

namespace lua {
namespace lexer {

namespace __parse_keyword_detail {
using namespace lua::tokens;

// Invariant:
//   keywords are ordered such that higher priority keywords come first
constexpr std::array<std::pair<keyword, std::string_view>, 21>
    ordered_keyword_string_rep{
        std::pair{keyword::AND, "and"},
        {keyword::BREAK, "break"},
        {keyword::DO, "do"},
        {keyword::ELSEIF, "elseif"},
        {keyword::ELSE, "else"},
        {keyword::END, "end"},
        {keyword::FALSE, "false"},
        {keyword::FOR, "for"},
        {keyword::FUNCTION, "function"},
        {keyword::IF, "if"},
        {keyword::IN, "in"},
        {keyword::LOCAL, "local"},
        {keyword::NIL, "nil"},
        {keyword::NOT, "not"},
        {keyword::OR, "or"},
        {keyword::REPEAT, "repeat"},
        {keyword::RETURN, "return"},
        {keyword::THEN, "then"},
        {keyword::TRUE, "true"},
        {keyword::UNTIL, "until"},
        {keyword::WHILE, "while"},
    };
}  // namespace __parse_keyword_detail

using __parse_keyword_detail::ordered_keyword_string_rep;

// Postcondition:
//   - parses if alphanumeric including _ prefix is a valid keyword
template <std::forward_iterator Iter>
constexpr auto parse_keyword(Iter begin, Iter end) -> std::optional<Iter> {
  auto keywords = std::views::transform(
      ordered_keyword_string_rep, [](auto const& rep) { return rep.second; });

  auto alpha_end = std::find_if_not(begin, end, is_alphanumeric_or_);
  auto keyword_itr = std::ranges::find_if(keywords, [=](auto keyword) {
    return std::equal(begin, alpha_end, std::begin(keyword), std::end(keyword));
  });

  if (keyword_itr == std::end(keywords))
    return std::nullopt;

  return alpha_end;
}

}  // namespace lexer
}  // namespace lua
