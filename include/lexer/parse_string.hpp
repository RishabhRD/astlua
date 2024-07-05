#pragma once

#include <algorithm>
#include <iterator>
#include <optional>
#include "algorithm.hpp"

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

}  // namespace lexer
}  // namespace lua
