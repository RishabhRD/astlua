#pragma once

#include <iterator>
#include <ranges>
#include "lexer/commons.hpp"
#include "token.hpp"

namespace lua {
namespace lexer {

namespace __parse_symbol_details {
using namespace lua::tokens;
// Invariant:
//   - symbol with common prefix have relative descending order
constexpr std::array<std::pair<symbol, std::string_view>, 26>
    ordered_symbol_string_rep{
        std::pair{symbol::PLUS, "+"},
        {symbol::MINUS, "-"},
        {symbol::PROD, "*"},
        {symbol::DIV, "/"},
        {symbol::MOD, "%"},
        {symbol::EXP, "^"},
        {symbol::LEN, "#"},
        {symbol::EQ, "=="},
        {symbol::NE, "~="},
        {symbol::LTE, "<="},
        {symbol::GTE, ">="},
        {symbol::LT, "<"},
        {symbol::GT, ">"},
        {symbol::ASSIGN, "="},
        {symbol::LPAREN, "("},
        {symbol::RPAREN, ")"},
        {symbol::LBRACE, "{"},
        {symbol::RBRACE, "}"},
        {symbol::LBRACKET, "["},
        {symbol::RBRACKET, "]"},
        {symbol::SEMICOLON, ";"},
        {symbol::COLON, ":"},
        {symbol::COMMA, ","},
        {symbol::VARARG, "..."},
        {symbol::CONCAT, ".."},
        {symbol::MEMBER, "."},
    };
}  // namespace __parse_symbol_details

using __parse_symbol_details::ordered_symbol_string_rep;

// Postcondition:
//   - parses first matching symbol in ordered_symbol_string_rep
template <std::forward_iterator Iter>
constexpr auto parse_symbol(Iter begin, Iter end) -> std::optional<Iter> {
  auto symbols = std::views::transform(
      ordered_symbol_string_rep, [](auto const& rep) { return rep.second; });
  return parse_contains(begin, end, std::begin(symbols), std::end(symbols));
}

}  // namespace lexer
}  // namespace lua
