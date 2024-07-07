#pragma once

#include <iterator>
#include "ast/ast.hpp"
#include "parser/combinator.hpp"
#include "parser/parsed.hpp"
#include "token/token.hpp"

namespace lua::parser {
inline auto parse_false = match(token::keyword::FALSE, ast::false_t{});
inline auto parse_true = match(token::keyword::TRUE, ast::true_t{});
inline auto parse_nil = match(token::keyword::NIL, ast::nil{});
inline auto parse_vararg = match(token::symbol::VARARG, ast::vararg{});

template <std::input_iterator Iter>
auto parse_binary_op(Iter begin, Iter end) -> parsed<ast::binary_op, Iter> {
  if (begin == end)
    return std::nullopt;

  if (*begin == token::token_t{token::symbol::PLUS})
    return std::pair{ast::binary_op::PLUS, ++begin};
  if (*begin == token::token_t{token::symbol::MINUS})
    return std::pair{ast::binary_op::MINUS, ++begin};
  if (*begin == token::token_t{token::symbol::PROD})
    return std::pair{ast::binary_op::PROD, ++begin};
  if (*begin == token::token_t{token::symbol::DIV})
    return std::pair{ast::binary_op::DIV, ++begin};
  if (*begin == token::token_t{token::symbol::EXP})
    return std::pair{ast::binary_op::EXP, ++begin};
  if (*begin == token::token_t{token::symbol::MOD})
    return std::pair{ast::binary_op::MOD, ++begin};
  if (*begin == token::token_t{token::symbol::CONCAT})
    return std::pair{ast::binary_op::CONCAT, ++begin};
  if (*begin == token::token_t{token::symbol::LT})
    return std::pair{ast::binary_op::LT, ++begin};
  if (*begin == token::token_t{token::symbol::LTE})
    return std::pair{ast::binary_op::LTE, ++begin};
  if (*begin == token::token_t{token::symbol::GT})
    return std::pair{ast::binary_op::GT, ++begin};
  if (*begin == token::token_t{token::symbol::GTE})
    return std::pair{ast::binary_op::GTE, ++begin};
  if (*begin == token::token_t{token::symbol::EQ})
    return std::pair{ast::binary_op::EQ, ++begin};
  if (*begin == token::token_t{token::symbol::NE})
    return std::pair{ast::binary_op::NE, ++begin};
  if (*begin == token::token_t{token::keyword::AND})
    return std::pair{ast::binary_op::AND, ++begin};
  if (*begin == token::token_t{token::keyword::OR})
    return std::pair{ast::binary_op::OR, ++begin};
  return std::nullopt;
}

template <std::input_iterator Iter>
auto parse_unary_op(Iter begin, Iter end) -> parsed<ast::unary_op, Iter> {
  if (begin == end)
    return std::nullopt;

  if (*begin == token::token_t{token::symbol::MINUS})
    return std::pair{ast::unary_op::MINUS, ++begin};
  if (*begin == token::token_t{token::keyword::NOT})
    return std::pair{ast::unary_op::NOT, ++begin};
  if (*begin == token::token_t{token::symbol::LEN})
    return std::pair{ast::unary_op::LEN, ++begin};
  return std::nullopt;
}

template <std::input_iterator Iter>
auto parse_field_sep(Iter begin, Iter end) -> parsed<ast::field_sep, Iter> {
  if (begin == end)
    return std::nullopt;

  if (*begin == token::token_t{token::symbol::COMMA})
    return std::pair{ast::field_sep::COMMA, ++begin};
  if (*begin == token::token_t{token::symbol::SEMICOLON})
    return std::pair{ast::field_sep::SEMICOLON, ++begin};
  return std::nullopt;
}
}  // namespace lua::parser
