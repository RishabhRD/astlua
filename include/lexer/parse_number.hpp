#pragma once

#include <algorithm>
#include <cctype>
#include "lexer/char_utils.hpp"

namespace lua {
namespace lexer {
namespace __parse_number_details {
template <std::input_iterator Iter>
constexpr auto parse_exponent_state(Iter begin,
                                    Iter end) -> std::optional<Iter> {
  if (begin == end)
    return std::nullopt;
  if (*begin == '+' || *begin == '-')
    ++begin;
  if (begin == end)
    return std::nullopt;
  if (!is_digit(*begin)) {
    return std::nullopt;
  }
  return std::find_if_not(begin, end, is_digit);
}

template <std::input_iterator Iter>
constexpr auto parse_decimal_fractional_state(Iter begin,
                                              Iter end) -> std::optional<Iter> {
  begin = std::find_if_not(begin, end, is_digit);
  if (begin == end)
    return begin;
  if (*begin == 'e' || *begin == 'E')
    return parse_exponent_state(++begin, end);
  return begin;
}

template <std::input_iterator Iter>
constexpr auto parse_decimal_integer_state(Iter begin,
                                           Iter end) -> std::optional<Iter> {
  begin = std::find_if_not(begin, end, is_digit);
  if (begin == end)
    return begin;
  if (*begin == '.')
    return parse_decimal_fractional_state(++begin, end);
  if (*begin == 'e' || *begin == 'E')
    return parse_exponent_state(++begin, end);
  return begin;
}

template <std::input_iterator Iter>
constexpr auto parse_decimal_start_state(Iter begin,
                                         Iter end) -> std::optional<Iter> {
  if (begin == end)
    return std::nullopt;
  if (is_digit(*begin))
    return parse_decimal_integer_state(++begin, end);
  else if (*begin == '.')
    return parse_decimal_fractional_state(++begin, end);

  return std::nullopt;
}

template <std::forward_iterator Iter>
constexpr auto parse_hex_fractional_state(
    Iter begin, Iter end, bool digit_found) -> std::optional<Iter> {
  if (begin == end) {
    if (digit_found)
      return begin;
    return std::nullopt;
  }

  auto tmp_iter = std::find_if_not(begin, end, is_hex_digit);
  digit_found = digit_found || (begin != tmp_iter);
  begin = tmp_iter;

  if (*begin == 'p' || *begin == 'P') {
    if (digit_found)
      return parse_exponent_state(++begin, end);
    return std::nullopt;
  }
  if (digit_found)
    return begin;
  return std::nullopt;
}

template <std::forward_iterator Iter>
constexpr auto parse_hex_integer_state(Iter begin,
                                       Iter end) -> std::optional<Iter> {
  if (begin == end)
    return std::nullopt;
  auto tmp_iter = std::find_if_not(begin, end, is_hex_digit);

  auto const digit_found = (begin != tmp_iter);
  begin = tmp_iter;
  if (*begin == '.')
    return parse_hex_fractional_state(++begin, end, digit_found);
  if (*begin == 'p' || *begin == 'P') {
    if (digit_found)
      return parse_exponent_state(++begin, end);
    return std::nullopt;
  }
  if (digit_found)
    return begin;
  return std::nullopt;
}

template <std::forward_iterator Iter>
constexpr auto parse_hex_start_state(Iter begin,
                                     Iter end) -> std::optional<Iter> {
  if (begin == end)
    return std::nullopt;
  if (*begin == '0')
    ++begin;
  else
    return std::nullopt;
  if (begin == end)
    return std::nullopt;
  if (*begin == 'x' || *begin == 'X')
    ++begin;
  else
    return std::nullopt;

  return parse_hex_integer_state(begin, end);
}

}  // namespace __parse_number_details

// Postcondition:
// Parses if prefix follows following grammar
//   Integer: [0-9]+
//   Fractional: [0-9]*\.[0-9]+ or [0-9]+\.[0-9]*
//   Scientific notation: ([0-9]+(\.[0-9]*)?|\.[0-9]+)[eE][+-]?[0-9]+
// and is not followed by any alphabet character or _
template <std::input_iterator Iter>
constexpr auto parse_decimal(Iter begin, Iter end) -> std::optional<Iter> {
  auto res = __parse_number_details::parse_decimal_start_state(begin, end);
  if (res && (*res == end || !is_alpha_or_(**res)))
    return res;
  return std::nullopt;
}

// Postcondition:
// Parses if prefix follows following grammar
//   Integer: 0[xX][0-9a-fA-F]+
//   Fractional: 0[xX]([0-9a-fA-F]*\.[0-9a-fA-F]+|[0-9a-fA-F]+\.[0-9a-fA-F]*)
//   Scientific notation:
//     0[xX]([0-9a-fA-F]+(\.[0-9a-fA-F]*)?|\.[0-9a-fA-F]+)[pP][+-]?[0-9]+
// and is not followed by any alphabet character or _
template <std::input_iterator Iter>
constexpr auto parse_hex(Iter begin, Iter end) -> std::optional<Iter> {
  auto res = __parse_number_details::parse_hex_start_state(begin, end);
  if (res && (*res == end || !is_alpha_or_(**res)))
    return res;
  return std::nullopt;
}

}  // namespace lexer
}  // namespace lua
