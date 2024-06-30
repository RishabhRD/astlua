#pragma once

#include "functional.hpp"
#include "lexer/char_utils.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <concepts>
#include <functional>
#include <string_view>

namespace astlua {
namespace lexer {
namespace __details {

constexpr bool is_valid_number_append(char c) {
  constexpr static std::array valid_append{'-', '*', '/', '%', '^', '=',
                                           '<', '>', ';', '+', ','};
  return std::isspace(c) || (std::find(begin(valid_append), end(valid_append),
                                       c) != end(valid_append));
}

template <std::input_iterator Iter, std::predicate<char> Pred>
constexpr auto parse_exponent_state(Iter begin, Iter end,
                                    Pred is_digit) -> std::optional<Iter> {
  if (begin == end)
    return std::nullopt;
  if (*begin == '+' || *begin == '-')
    ++begin;
  if (begin == end)
    return std::nullopt;
  if (!is_digit(*begin)) {
    return std::nullopt;
  }
  return std::find_if(begin, end, std::not_fn(is_digit));
}

template <std::input_iterator Iter>
constexpr auto parse_decimal_exponent_state(Iter begin, Iter end) {
  return parse_exponent_state(begin, end, lift(is_digit));
}

template <std::input_iterator Iter>
constexpr auto parse_hex_exponent_state(Iter begin, Iter end) {
  return parse_exponent_state(begin, end, lift(is_hex_digit));
}

template <std::input_iterator Iter>
constexpr auto parse_decimal_fractional_state(Iter begin,
                                              Iter end) -> std::optional<Iter> {
  begin = std::find_if(begin, end, std::not_fn(lift(is_digit)));
  if (begin == end)
    return begin;
  if (*begin == 'e' || *begin == 'E')
    return parse_decimal_exponent_state(++begin, end);
  return begin;
}

template <std::input_iterator Iter>
constexpr auto parse_decimal_integer_state(Iter begin,
                                           Iter end) -> std::optional<Iter> {
  begin = std::find_if(begin, end, std::not_fn(lift(is_digit)));
  if (begin == end)
    return begin;
  if (*begin == '.')
    return parse_decimal_fractional_state(++begin, end);
  if (*begin == 'e' || *begin == 'E')
    return parse_decimal_exponent_state(++begin, end);
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
constexpr auto
parse_hex_fractional_state(Iter begin, Iter end,
                           bool digit_found) -> std::optional<Iter> {
  if (begin == end) {
    if (digit_found)
      return begin;
    return std::nullopt;
  }

  auto tmp_iter = std::find_if(begin, end, std::not_fn(lift(is_hex_digit)));
  digit_found = digit_found || (begin != tmp_iter);
  ++begin;

  if (*begin == 'p' || *begin == 'P') {
    if (digit_found)
      return parse_hex_exponent_state(++begin, end);
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
  auto tmp_iter = std::find_if(begin, end, std::not_fn(lift(is_hex_digit)));

  auto const digit_found = (begin != tmp_iter);
  begin = tmp_iter;
  if (*begin == '.')
    return parse_hex_fractional_state(++begin, end, digit_found);
  if (*begin == 'p' || *begin == 'P') {
    if (digit_found)
      return parse_hex_exponent_state(++begin, end);
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

} // namespace __details

// Postcondition:
// Returns iterator pointing to element after prefix following grammar:
//   Integer: [0-9]+
//   Fractional: [0-9]*\.[0-9]+ or [0-9]+\.[0-9]*
//   Scientific notation: ([0-9]+(\.[0-9]*)?|\.[0-9]+)[eE][+-]?[0-9]+
// if followed by end iterator or character following is_valid_number_append
// otherwise nullopt
template <std::input_iterator Iter>
constexpr auto parse_decimal(Iter begin, Iter end) -> std::optional<Iter> {
  auto res = __details::parse_decimal_start_state(begin, end);
  if (!res)
    return std::nullopt;
  if (*res == end || __details::is_valid_number_append(**res))
    return res;
  return std::nullopt;
}

// Postcondition:
// distance of iterator (begin, parse_decimal(begin, end)) if returned
// otherwise null
constexpr auto
parse_decimal(std::string_view rng) -> std::optional<std::size_t> {
  auto res = parse_decimal(begin(rng), end(rng));
  if (!res)
    return std::nullopt;
  return static_cast<std::size_t>(std::distance(begin(rng), *res));
}

// Postcondition:
// Returns iterator pointing to element after prefix following grammar:
//   Integer: [0-9]+
//   Fractional: [0-9]*\.[0-9]+ or [0-9]+\.[0-9]*
//   Scientific notation: ([0-9]+(\.[0-9]*)?|\.[0-9]+)[eE][+-]?[0-9]+
// if followed by end iterator or character following is_valid_number_append
// otherwise nullopt
template <std::input_iterator Iter>
constexpr auto parse_hex(Iter begin, Iter end) -> std::optional<Iter> {
  auto res = __details::parse_hex_start_state(begin, end);
  if (!res)
    return std::nullopt;
  if (*res == end || __details::is_valid_number_append(**res))
    return res;
  return std::nullopt;
}

// Postcondition:
// distance of iterator (begin, parse_decimal(begin, end)) if returned
// otherwise null
constexpr auto parse_hex(std::string_view rng) -> std::optional<std::size_t> {
  auto res = parse_hex(begin(rng), end(rng));
  if (!res)
    return std::nullopt;
  return static_cast<std::size_t>(std::distance(begin(rng), *res));
}

} // namespace lexer
} // namespace astlua
