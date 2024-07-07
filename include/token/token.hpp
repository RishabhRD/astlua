#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <variant>

namespace lua {
namespace token {
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

std::ostream& operator<<(std::ostream&, keyword);
std::ostream& operator<<(std::ostream&, symbol);

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

using token_t =
    std::variant<keyword, symbol, string, identifier, number, illegal>;

std::ostream& operator<<(std::ostream& os, token_t const& token_var);
}  // namespace token
}  // namespace lua
