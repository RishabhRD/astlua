#pragma once

#include <string>
#include <variant>
namespace astlua {
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

struct string {
  std::string value;

  friend bool operator==(string const &, string const &) = default;
};

struct identifier {
  std::string value;

  friend bool operator==(identifier const &, identifier const &) = default;
};

struct number {
  std::string value;

  friend bool operator==(number const &, number const &) = default;
};

struct illegal {
  friend bool operator==(illegal const &, illegal const &) = default;
};

} // namespace tokens
using token = std::variant<tokens::keyword, tokens::symbol, tokens::string,
                           tokens::identifier, tokens::number, tokens::illegal>;
} // namespace astlua
