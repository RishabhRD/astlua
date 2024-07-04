#pragma once

#include <array>
#include <string>
#include <variant>

namespace lua {
namespace tokens {
enum class keyword {
  AND,
  BREAK,
  DO,
  ELSE,
  ELSEIF,
  END,
  FALSE,
  FOR,
  FUNCTION,
  IF,
  IN,
  LOCAL,
  NIL,
  NOT,
  OR,
  REPEAT,
  RETURN,
  THEN,
  TRUE,
  UNTIL,
  WHILE,
};

enum class symbol {
  PLUS,
  MINUS,
  PROD,
  DIV,
  MOD,
  EXP,
  LEN,
  EQ,
  NE,
  LTE,
  GTE,
  LT,
  GT,
  ASSIGN,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  LBRACKET,
  RBRACKET,
  SEMICOLON,
  COLON,
  COMMA,
  MEMBER,
  CONCAT,
  VARARG,
};

// Invariant:
//   - symbol with common prefix have relative descending order
constexpr std::array<std::pair<symbol, std::string_view>, 26> symbol_string_rep{
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

struct string {
  std::string value;

  friend bool operator==(string const&, string const&) = default;
};

struct identifier {
  std::string value;

  friend bool operator==(identifier const&, identifier const&) = default;
};

struct number {
  std::string value;

  friend bool operator==(number const&, number const&) = default;
};

struct illegal {
  friend bool operator==(illegal const&, illegal const&) = default;
};

}  // namespace tokens

using token = std::variant<tokens::keyword, tokens::symbol, tokens::string,
                           tokens::identifier, tokens::number, tokens::illegal>;
}  // namespace lua
