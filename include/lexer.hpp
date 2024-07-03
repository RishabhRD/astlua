#pragma once

#include "functional.hpp"
#include "lexer/parse_number.hpp"
#include "position.hpp"
#include "token.hpp"
#include "token_info.hpp"
#include <algorithm>
#include <cctype>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

namespace lua {
namespace __lexer_details {
template <std::input_iterator Iter>
constexpr auto reduce_position(Iter begin, Iter end, position cur_position) {
  return std::accumulate(begin, end, cur_position, [](position cur, char c) {
    if (c == '\n') {
      ++cur.line;
      cur.col = 0;
    } else {
      ++cur.col;
    }
    return cur;
  });
}

template <std::ranges::input_range Range>
constexpr auto reduce_position(Range const &rng, position cur_position) {
  return reduce_position(std::ranges::begin(rng), std::ranges::end(rng),
                         cur_position);
}

constexpr auto cnt_prefix_whitespace(std::string_view str) -> std::size_t {
  return static_cast<std::size_t>(
      std::ranges::count_if(str, lift(std::isspace)));
}

constexpr auto is_prefix(std::string_view prefix,
                         std::string_view str) -> bool {
  return prefix.size() <= str.size() && str.substr(0, prefix.size()) == prefix;
}

constexpr auto extract_number(std::string_view str)
    -> std::optional<std::pair<token, std::size_t>> {
  return lexer::parse_decimal(str)                      //
      .or_else([str] { return lexer::parse_hex(str); }) //
      .transform([str](auto len) {
        return std::pair{tokens::number{std::string{str.substr(0, len)}}, len};
      });
}

constexpr auto extract_keyword(std::string_view str)
    -> std::optional<std::pair<token, std::size_t>> {
  auto token_itr =
      std::ranges::find_if(tokens::keyword_string_rep, [str](auto const &rep) {
        return is_prefix(rep.second, str);
      });
  if (token_itr == std::ranges::end(tokens::keyword_string_rep))
    return std::nullopt;
  return std::pair{token_itr->first, token_itr->second.size()};
}

constexpr auto extract_symbol(std::string_view str)
    -> std::optional<std::pair<token, std::size_t>> {
  auto token_itr = std::ranges::find_if(
      tokens::symbol_string_rep,
      [str](std::pair<tokens::symbol, std::string_view> const &rep) {
        return is_prefix(rep.second, str);
      });
  if (token_itr == std::ranges::end(tokens::symbol_string_rep))
    return std::nullopt;
  return std::pair{token_itr->first, token_itr->second.size()};
}

// Precondition:
//   - str.size() > 0
constexpr auto
extract_token(std::string_view str) -> std::pair<token, std::size_t> {
  return extract_keyword(str)
      .or_else([str] { return extract_number(str); })
      .or_else([str] { return extract_symbol(str); })
      .value_or(std::pair{tokens::illegal{}, 1});
}

} // namespace __lexer_details

constexpr auto lex_source(std::string_view source) -> std::vector<token_info> {
  position cur_position{
      .line = 0,
      .col = 0,
  };
  auto advance_state = [&](std::size_t cnt) {
    cur_position =
        __lexer_details::reduce_position(source.substr(0, cnt), cur_position);
    source = source.substr(0, cnt);
  };
  std::vector<token_info> tokens;
  while (source.size()) {
    advance_state(__lexer_details::cnt_prefix_whitespace(source));
    if (source.size() == 0)
      break;
    auto [token, token_size] = __lexer_details::extract_token(source);
    tokens.push_back({token, cur_position});
    advance_state(token_size);
  }
  return tokens;
}
} // namespace lua
