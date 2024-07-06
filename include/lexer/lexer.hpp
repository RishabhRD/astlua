#pragma once

#include <iterator>
#include <string>
#include "lexer/parse_identifier.hpp"
#include "lexer/parse_keyword.hpp"
#include "lexer/parse_number.hpp"
#include "lexer/parse_string.hpp"
#include "lexer/parse_symbol.hpp"
#include "lexer/skip_non_tokens.hpp"
#include "position.hpp"
#include "token_info.hpp"

namespace lua {
namespace lexer {
template <std::input_iterator Iter>
constexpr auto reduce_position(Iter begin, Iter end, position_t cur_position) {
  char prev = '\0';
  while (begin != end) {
    if (*begin == '\r') {
      cur_position.col = 0;
      ++cur_position.line;
    } else if (*begin == '\n') {
      if (prev == '\r')
        continue;
      cur_position.col = 0;
      ++cur_position.line;
    } else {
      ++cur_position.col;
    }
  }
  return cur_position;
}

// Precondition:
//   - begin != end
//
// Postcondition:
//   - Returns the prefix token and iterator pointing in [begin, end) after
//     prefix token
//   - Precedence of tokens are in order:
//       - keyword -> parse_keyword
//       - number -> (parse_hex > parse_decimal)
//       - string -> (parse_long_string > parse_short_string)
//       - symbol -> parse_symbol
//       - identifer -> parse_identifer
//       - illegal
template <std::forward_iterator Iter>
constexpr auto extract_token(Iter begin,
                             Iter end) -> std::pair<lua::token_t, Iter> {
  using namespace lua::tokens;

  auto res = parse_keyword(begin, end);
  if (res)
    return {to_keyword(begin, *res), *res};

  res = parse_hex(begin, end);
  if (res)
    return {number{std::string{begin, *res}}, *res};

  res = parse_decimal(begin, end);
  if (res)
    return {number{std::string{begin, *res}}, *res};

  res = parse_long_string(begin, end);
  if (res)
    return {string{std::string{begin, *res}}, *res};

  res = parse_short_string(begin, end);
  if (res)
    return {string{std::string{begin, *res}}, *res};

  res = parse_symbol(begin, end);
  if (res)
    return {to_symbol(begin, *res), *res};

  res = parse_identifier(begin, end);
  if (res)
    return {identifier{std::string{begin, *res}}, *res};

  auto illegal_char = *begin;
  return {illegal{illegal_char}, ++begin};
}

template <std::forward_iterator Iter, std::forward_iterator Out>
  requires std::same_as<std::iter_value_t<Out>, token_info>
constexpr auto tokenize(Iter begin, Iter end, Out out) -> void {
  position_t cur_position{
      .line = 0,
      .col = 0,
  };

  while (begin != end) {
    auto cur = skip_non_tokens(begin, end);
    cur_position = reduce_position(begin, cur, cur_position);
    begin = cur_position;

    auto [token, new_begin] = extract_token(begin, end);
    *out = token_info{std::move(token), cur_position};
    ++out;

    cur_position = reduce_position(begin, new_begin, cur_position);
    begin = new_begin;
  }
}
}  // namespace lexer
}  // namespace lua
