#pragma once

#include <array>
#include <ostream>
#include "lexer/token.hpp"

namespace lua {
namespace lexer {
namespace __representation_details {
using namespace lua::lexer::tokens;

// Invariant:
//   keywords are ordered such that higher priority keywords come first
constexpr std::array<std::pair<keyword, std::string_view>, 21>
    ordered_keyword_string_rep{
        std::pair{keyword::AND, "and"},
        {keyword::BREAK, "break"},
        {keyword::DO, "do"},
        {keyword::ELSEIF, "elseif"},
        {keyword::ELSE, "else"},
        {keyword::END, "end"},
        {keyword::FALSE, "false"},
        {keyword::FOR, "for"},
        {keyword::FUNCTION, "function"},
        {keyword::IF, "if"},
        {keyword::IN, "in"},
        {keyword::LOCAL, "local"},
        {keyword::NIL, "nil"},
        {keyword::NOT, "not"},
        {keyword::OR, "or"},
        {keyword::REPEAT, "repeat"},
        {keyword::RETURN, "return"},
        {keyword::THEN, "then"},
        {keyword::TRUE, "true"},
        {keyword::UNTIL, "until"},
        {keyword::WHILE, "while"},
    };

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
}  // namespace __representation_details

using __representation_details::ordered_keyword_string_rep;
using __representation_details::ordered_symbol_string_rep;

// Precondition:
//   - [begin, end) is a valid keyword string
//
// Postcondition:
//   - Returns the keyword represented by [begin, end) in ordered_keyword_string_rep
template <std::forward_iterator Iter>
constexpr auto to_keyword(Iter begin, Iter end) -> lua::lexer::tokens::keyword {
  return std::ranges::find_if(ordered_keyword_string_rep,
                              [begin, end](auto const& rep) {
                                return std::equal(begin, end,
                                                  std::begin(rep.second),
                                                  std::end(rep.second));
                              })
      ->first;
  ;
}

// Precondition:
//   - [begin, end) is a valid symbol string
//
// Postcondition:
//   - Returns the symbol represented by [begin, end) in ordered_symbol_string_rep
template <std::forward_iterator Iter>
constexpr auto to_symbol(Iter begin, Iter end) -> lua::lexer::tokens::symbol {
  return std::ranges::find_if(ordered_symbol_string_rep,
                              [begin, end](auto const& rep) {
                                return std::equal(begin, end,
                                                  std::begin(rep.second),
                                                  std::end(rep.second));
                              })
      ->first;
  ;
}

namespace tokens {
inline std::ostream& operator<<(std::ostream& os,
                                lua::lexer::tokens::keyword k) {
  os << "keyword{";
  os << std::ranges::find_if(ordered_keyword_string_rep, [k](auto const& rep) {
          return rep.first == k;
        })->second;
  os << "}";
  return os;
}

inline std::ostream& operator<<(std::ostream& os,
                                lua::lexer::tokens::symbol k) {
  os << "symbol{";
  os << std::ranges::find_if(ordered_symbol_string_rep, [k](auto const& rep) {
          return rep.first == k;
        })->second;
  os << "}";
  return os;
}
}  // namespace tokens
}  // namespace lexer
}  // namespace lua
