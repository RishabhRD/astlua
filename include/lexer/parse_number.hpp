#pragma once

#include "functional.hpp"
#include "lexer/char_utils.hpp"
#include <algorithm>
#include <cctype>
#include <functional>
#include <string_view>

namespace astlua {
namespace __parse_number {

template <std::input_iterator Iter>
constexpr auto is_hexadecimal_number(Iter begin, Iter end) {
  if (begin == end)
    return false;
  if (*begin != '0')
    return false;
  ++begin;
  if (begin == end)
    return false;
  if (*begin != 'x')
    return false;
  return true;
}

template <std::forward_iterator Iter>
constexpr auto find_number(Iter begin, Iter end) -> std::optional<Iter> {
  auto const is_hex = is_hexadecimal_number(begin, end);
  auto is_valid_digit = [is_hex](char c) {
    return is_digit(c) || (is_hex && c >= 'a' && c <= 'f');
  };
  auto is_invalid_char = std::not_fn(is_valid_digit);
  if (is_hex) {
    std::advance(begin, 2);
  }

  auto res_iter = std::find_if(begin, end, is_invalid_char);
  if (begin == res_iter)
    return std::nullopt;
  begin = res_iter;

  if (begin == end)
    return begin;

  if (*begin == '.') {
    ++begin;
    begin = std::find_if(begin, end, is_invalid_char);
  }

  if (begin == end)
    return begin;

  if (*begin == 'e') {
    ++begin;
    if (begin == end)
      return std::nullopt;
    if (*begin == '-')
      ++begin;
    res_iter = std::find_if(begin, end, std::not_fn(lift(is_digit)));
    if (res_iter == begin)
      return std::nullopt;
    begin = res_iter;
  }

  if (begin == end || std::isspace(*begin))
    return begin;
  return std::nullopt;
}

template <std::forward_iterator Iter>
constexpr auto parse_number(Iter begin,
                            Iter end) -> std::optional<std::size_t> {
  return find_number(begin, end).transform([begin](auto res) {
    return static_cast<std::size_t>(std::distance(begin, res));
  });
}

constexpr auto
parse_number(std::string_view rng) -> std::optional<std::size_t> {
  return parse_number(std::begin(rng), std::end(rng));
}
} // namespace __parse_number
} // namespace astlua
