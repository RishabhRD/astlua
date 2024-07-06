#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <variant>
#include "functional.hpp"

namespace lua {
namespace lexer {
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

inline std::ostream& operator<<(std::ostream&, keyword);
inline std::ostream& operator<<(std::ostream&, symbol);

struct string {
  std::string value;

  friend bool operator==(string const&, string const&) = default;

  friend std::ostream& operator<<(std::ostream& os, string const& s) {
    os << "string{" << s.value << "}";
    return os;
  }
};

struct identifier {
  std::string value;

  friend bool operator==(identifier const&, identifier const&) = default;

  friend std::ostream& operator<<(std::ostream& os, identifier const& s) {
    os << "identifier{" << s.value << "}";
    return os;
  }
};

struct number {
  std::string value;

  friend bool operator==(number const&, number const&) = default;

  friend std::ostream& operator<<(std::ostream& os, number const& s) {
    os << "number{" << s.value << "}";
    return os;
  }
};

struct illegal {
  char value;

  friend bool operator==(illegal const&, illegal const&) = default;

  friend std::ostream& operator<<(std::ostream& os, illegal const& s) {
    os << "illegal{" << s.value << "}";
    return os;
  }
};

}  // namespace tokens

using token_t =
    std::variant<tokens::keyword, tokens::symbol, tokens::string,
                 tokens::identifier, tokens::number, tokens::illegal>;

inline std::ostream& operator<<(std::ostream& os, token_t const& token_var) {
  std::visit(nostd::overload{
                 [&os](tokens::keyword const& x) { os << x; },
                 [&os](tokens::symbol const& x) { os << x; },
                 [&os](tokens::string const& x) { os << x; },
                 [&os](tokens::identifier const& x) { os << x; },
                 [&os](tokens::number const& x) { os << x; },
                 [&os](tokens::illegal const& x) { os << x; },
             },
             token_var);
  return os;
}
}  // namespace lexer
}  // namespace lua
