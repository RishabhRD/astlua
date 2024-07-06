#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <optional>
#include "algorithm.hpp"
#include "lexer/char_utils.hpp"

namespace lua {
namespace lexer {

namespace __parse_string_details {

template <std::input_iterator Iter>
using diff_t = std::iter_difference_t<Iter>;

template <std::forward_iterator Iter>
constexpr auto is_long_string_closing(Iter begin, Iter end,
                                      diff_t<Iter> num_equal) {
  if (begin == end || *begin != ']')
    return false;
  ++begin;
  auto not_equal_itr =
      std::find_if_not(begin, end, [](auto c) { return c == '='; });
  if (std::distance(begin, not_equal_itr) != num_equal)
    return false;
  begin = not_equal_itr;
  return begin != end && *begin == ']';
}

template <std::forward_iterator Iter>
constexpr auto parse_long_string_body_state(
    Iter begin, Iter end, diff_t<Iter> num_equal) -> std::optional<Iter> {
  begin = nostd::find_if_iter(begin, end, [end, num_equal](auto cur_begin) {
    return is_long_string_closing(cur_begin, end, num_equal);
  });
  if (begin == end)
    return std::nullopt;
  std::advance(begin, num_equal + 2);
  return begin;
}

template <std::forward_iterator Iter>
constexpr auto parse_long_string_open_state(Iter begin,
                                            Iter end) -> std::optional<Iter> {
  auto not_equal_itr =
      std::find_if_not(begin, end, [](auto c) { return c == '='; });
  auto num_equal = std::distance(begin, not_equal_itr);
  begin = not_equal_itr;
  if (begin != end && *begin == '[')
    return parse_long_string_body_state(++begin, end, num_equal);
  return std::nullopt;
}

template <std::forward_iterator Iter>
constexpr auto parse_long_string_start_state(Iter begin,
                                             Iter end) -> std::optional<Iter> {
  if (begin != end && *begin == '[')
    return parse_long_string_open_state(++begin, end);
  return std::nullopt;
}

template <std::forward_iterator Iter>
constexpr auto parse_escape_seq(Iter begin, Iter end) -> std::optional<Iter> {
  if (begin == end)
    return std::nullopt;
  std::array single_escape{'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '"', '\'',
                           '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9'};
  if (std::find(std::begin(single_escape), std::end(single_escape), *begin) !=
      std::end(single_escape)) {
    return ++begin;
  }
  if (*begin == 'x') {
    ++begin;
    if (begin == end)
      return std::nullopt;
    if (is_hex_digit(*begin))
      return ++begin;
    return std::nullopt;
  }
  if (*begin == 'u') {
    ++begin;
    if (begin == end)
      return std::nullopt;
    if (*begin != '{')
      return std::nullopt;
    ++begin;
    auto non_hex_itr = std::find_if_not(begin, end, is_hex_digit);
    auto dist = std::distance(begin, non_hex_itr);
    if (dist == 0)
      return std::nullopt;
    if (dist > 6)
      return std::nullopt;
    if (dist == 6 && std::all_of(std::next(begin, 2), non_hex_itr,
                                 [](auto c) { return c == 'f' || c == 'F'; })) {
      bool const is_good = (*begin == '0' && *std::next(begin) == '0') ||
                           (*begin == '0' && *std::next(begin) == '1') ||
                           (*begin == '1' && *std::next(begin) == '0');
      if (!is_good)
        return std::nullopt;
    }
    begin = non_hex_itr;
    if (begin == end)
      return std::nullopt;
    if (*begin == '}')
      return ++begin;
    return std::nullopt;
  }
  return std::nullopt;
}
}  // namespace __parse_string_details

// Postcondition:
// Parses if the prefix [begin, end) follows following grammar:
//   LONG_STRING ::= LONG_STRING_OPEN LONG_STRING_CONTENT LONG_STRING_CLOSE
//   LONG_STRING_OPEN ::= '[' LONG_STRING_EQUALS '['
//   LONG_STRING_CLOSE ::= ']' LONG_STRING_EQUALS ']'
//   LONG_STRING_EQUALS ::= '='*
//   LONG_STRING_CONTENT ::= ( [^]] | ']' ( [^=]* '=' ] ) )*
template <std::forward_iterator Iter>
constexpr auto parse_long_string(Iter begin, Iter end) -> std::optional<Iter> {
  return __parse_string_details::parse_long_string_start_state(begin, end);
}

// Postcondition:
// Parses if the prefix [begin, end) follows following grammar:
//   SHORT_STRING ::= SINGLE_QUOTE_STRING | DOUBLE_QUOTE_STRING
//   SINGLE_QUOTE_STRING ::= "'" ( SHORT_CHAR | ESCAPE_SEQUENCE )* "'"
//   DOUBLE_QUOTE_STRING ::= '"' ( SHORT_CHAR | ESCAPE_SEQUENCE )* '"'
//   SHORT_CHAR ::= [^'\n\r\\]
//   ESCAPE_SEQUENCE ::= '\' ( ESCAPE_CHAR | DIGIT_SEQUENCE | HEX_SEQUENCE | UNICODE_SEQUENCE )
//   ESCAPE_CHAR ::= 'a' | 'b' | 'f' | 'n' | 'r' | 't' | 'v' | '\\' | '"' | "'"
//   DIGIT_SEQUENCE ::= DIGIT DIGIT? DIGIT?  # Up to three decimal digits
//   HEX_SEQUENCE ::= 'x' HEX_DIGIT HEX_DIGIT  # Two hexadecimal digits
//   UNICODE_SEQUENCE ::= 'u{' UNICODE_HEX_DIGIT+ '}'  # Unicode sequence inside braces
//   UNICODE_HEX_DIGIT ::= [0-9A-Fa-f]
//   DIGIT ::= '0' - '9'
//   HEX_DIGIT ::= DIGIT | 'A' - 'F' | 'a' - 'f'
// And unicode sequence should be between \u{000000} to \u{00ffff}
template <std::forward_iterator Iter>
constexpr auto parse_short_string(Iter begin, Iter end) -> std::optional<Iter> {
  if (begin == end || (*begin != '"' && *begin != '\''))
    return std::nullopt;
  auto const start_char = *begin;
  ++begin;
  bool found_end = false;
  while (begin != end) {
    if (*begin == '\\') {
      auto res = __parse_string_details::parse_escape_seq(++begin, end);
      if (!res)
        return std::nullopt;
      begin = *res;
    } else if (*begin == start_char) {
      ++begin;
      found_end = true;
      break;
    } else if (*begin == '\n' || *begin == '\r') {
      return std::nullopt;
    } else {
      ++begin;
    }
  }
  if (found_end)
    return begin;
  return std::nullopt;
}

}  // namespace lexer
}  // namespace lua
