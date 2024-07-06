#pragma once

#include <algorithm>
#include <iterator>
#include <optional>
#include "char_utils.hpp"
#include "functional.hpp"
#include "lexer/parse_string.hpp"

namespace lua {
namespace lexer {
namespace __skip_non_tokens_details {
template <std::forward_iterator Iter>
constexpr auto skip_whitespace(Iter begin, Iter end) -> Iter {
  return std::find_if_not(begin, end, lift(std::isspace));
}

template <std::forward_iterator Iter>
constexpr auto parse_comment(Iter begin, Iter end) -> std::optional<Iter> {
  if (begin == end || *begin != '-')
    return std::nullopt;
  ++begin;
  if (begin == end || *begin != '-')
    return std::nullopt;
  auto long_string_itr = parse_long_string(begin, end);
  if (long_string_itr != std::nullopt)
    return long_string_itr;
  begin = std::find_if(begin, end, is_newline);
  if (begin != end)
    ++begin;
  return begin;
}
}  // namespace __skip_non_tokens_details

template <std::forward_iterator Iter>
constexpr auto skip_non_tokens(Iter begin, Iter end) -> Iter {
  using namespace __skip_non_tokens_details;
  while (true) {
    auto new_begin = skip_whitespace(begin, end);
    auto c_end = parse_comment(new_begin, end);
    if (c_end)
      new_begin = *c_end;
    if (new_begin == begin)
      break;
    begin = new_begin;
  }
  return begin;
}
}  // namespace lexer
}  // namespace lua
