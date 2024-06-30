#pragma once

#include "functional.hpp"
#include "lexer/char_utils.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <functional>
#include <string_view>

namespace astlua {
namespace lexer {
namespace __details {

constexpr bool is_valid_number_append(char c) {
  constexpr static std::array valid_append{'-', '*', '/', '%', '^',
                                           '=', '<', '>', ';', '+'};
  return std::isspace(c) || (std::find(begin(valid_append), end(valid_append),
                                       c) != end(valid_append));
}

template <std::input_iterator Iter>
constexpr auto parse_decimal_exponent_state(Iter begin,
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
  return std::find_if(begin, end, std::not_fn(lift(is_digit)));
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
} // namespace __details

template <std::input_iterator Iter>
constexpr auto parse_decimal(Iter begin, Iter end) -> std::optional<Iter> {
  auto res = __details::parse_decimal_start_state(begin, end);
  if (!res)
    return std::nullopt;
  if (*res == end || __details::is_valid_number_append(**res))
    return res;
  return std::nullopt;
}

constexpr auto
parse_decimal(std::string_view rng) -> std::optional<std::size_t> {
  auto res = parse_decimal(begin(rng), end(rng));
  if (!res)
    return std::nullopt;
  return static_cast<std::size_t>(std::distance(begin(rng), *res));
}

} // namespace lexer
} // namespace astlua
