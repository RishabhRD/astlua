#pragma once

#include <iterator>
#include "ast/ast.hpp"
#include "parser/parsed.hpp"
#include "token/token.hpp"

namespace lua::parser {
template <std::input_iterator Iter>
auto parse_false(Iter begin, Iter end) -> parsed<ast::false_t, Iter> {
  if (begin == end)
    return std::nullopt;

  if (*begin == token::token_t{token::keyword::FALSE})
    return std::pair{ast::false_t{}, ++begin};
  return std::nullopt;
}

template <std::input_iterator Iter>
auto parse_true(Iter begin, Iter end) -> parsed<ast::true_t, Iter> {
  if (begin == end)
    return std::nullopt;

  if (*begin == token::token_t{token::keyword::TRUE})
    return std::pair{ast::true_t{}, ++begin};
  return std::nullopt;
}

template <std::input_iterator Iter>
auto parse_nil(Iter begin, Iter end) -> parsed<ast::nil, Iter> {
  if (begin == end)
    return std::nullopt;

  if (*begin == token::token_t{token::keyword::NIL})
    return std::pair{ast::nil{}, ++begin};
  return std::nullopt;
}

template <std::input_iterator Iter>
auto parse_vararg(Iter begin, Iter end) -> parsed<ast::vararg, Iter> {
  if (begin == end)
    return std::nullopt;

  if (*begin == token::token_t{token::symbol::VARARG})
    return std::pair{ast::vararg{}, ++begin};
  return std::nullopt;
}

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
