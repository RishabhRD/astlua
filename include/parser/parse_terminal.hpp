#pragma once

#include "ast/ast.hpp"
#include "parser/combinator.hpp"
#include "token/token.hpp"

namespace lua::parser {
inline auto parse_false = match(token::keyword::FALSE, ast::false_t{});
inline auto parse_true = match(token::keyword::TRUE, ast::true_t{});
inline auto parse_nil = match(token::keyword::NIL, ast::nil{});
inline auto parse_vararg = match(token::symbol::VARARG, ast::vararg{});
inline auto parse_binary_op =
    choice<ast::binary_op>(match(token::symbol::PLUS, ast::binary_op::PLUS),
                           match(token::symbol::MINUS, ast::binary_op::MINUS),
                           match(token::symbol::PROD, ast::binary_op::PROD),
                           match(token::symbol::DIV, ast::binary_op::DIV),
                           match(token::symbol::EXP, ast::binary_op::EXP),
                           match(token::symbol::MOD, ast::binary_op::MOD),
                           match(token::symbol::CONCAT, ast::binary_op::CONCAT),
                           match(token::symbol::LT, ast::binary_op::LT),
                           match(token::symbol::LTE, ast::binary_op::LTE),
                           match(token::symbol::GT, ast::binary_op::GT),
                           match(token::symbol::GTE, ast::binary_op::GTE),
                           match(token::symbol::EQ, ast::binary_op::EQ),
                           match(token::symbol::NE, ast::binary_op::NE),
                           match(token::keyword::AND, ast::binary_op::AND),
                           match(token::keyword::OR, ast::binary_op::OR));
inline auto parse_unary_op =
    choice<ast::unary_op>(match(token::symbol::MINUS, ast::unary_op::MINUS),
                          match(token::keyword::NOT, ast::unary_op::NOT),
                          match(token::symbol::LEN, ast::unary_op::LEN));
inline auto parse_field_sep = choice<ast::field_sep>(
    match(token::symbol::COMMA, ast::field_sep::COMMA),
    match(token::symbol::SEMICOLON, ast::field_sep::SEMICOLON));
}  // namespace lua::parser
