#pragma once

#include <variant>
#include "ast/ast.hpp"
#include "parser/combinator.hpp"
#include "token/token.hpp"

namespace lua::parser {
// Postcondition:
//  - get 2nd argument for 2 argument function
inline auto get_snd2 = [](auto const&, auto val) {
  return val;
};

inline auto name_parser = match_if_then(
    [](auto const& name_token) {
      return std::holds_alternative<token::identifier>(name_token);
    },
    [](auto name_token) {
      return std::get<token::identifier>(name_token).value;
    });

inline auto list_parser(auto parser) {
  auto comma_val_parser =
      sequence(get_snd2, skip(token::symbol::COMMA), parser);
  return sequence(
      [](auto val, auto lst) {
        return ast::list_1{std::move(val), std::move(lst)};
      },
      std::move(parser), zero_or_more(std::move(comma_val_parser)));
}

inline auto name_list_parser = list_parser(name_parser);

inline auto number_parser = match_if_then(
    [](auto const& token) {
      return std::holds_alternative<token::number>(token);
    },
    [](auto token) {
      return ast::number{std::get<token::number>(token).value};
    });

inline auto string_parser = match_if_then(
    [](auto const& token) {
      return std::holds_alternative<token::string>(token);
    },
    [](auto token) {
      return ast::string{std::get<token::string>(token).value};
    });

inline auto nil_parser = match(token::keyword::NIL, ast::nil());
inline auto true_parser = match(token::keyword::TRUE, ast::true_t());
inline auto false_parser = match(token::keyword::FALSE, ast::false_t());
inline auto vararg_parser = match(token::symbol::VARARG, ast::vararg());

inline auto expr_parser =
    choice<ast::expr>(number_parser, string_parser, nil_parser, true_parser,
                      false_parser, vararg_parser);
inline auto expr_list_parser = list_parser(expr_parser);

inline auto fn_name_parser = sequence(
    [](auto name, auto dot_names, auto colon_name) {
      return ast::fn_name{std::move(name), std::move(dot_names),
                          std::move(colon_name)};
    },
    name_parser,
    zero_or_more(sequence(get_snd2, skip(token::symbol::MEMBER), name_parser)),
    maybe(sequence(get_snd2, skip(token::symbol::COLON), name_parser)));

inline auto field_sep_parser = choice<std::monostate>(
    skip(token::symbol::COMMA), skip(token::symbol::SEMICOLON));

inline auto expr_field_parser = sequence(
    [](auto, auto key, auto, auto, auto value) {
      return ast::expr_field{std::move(key), std::move(value)};
    },
    skip(token::symbol::LBRACKET), expr_parser, skip(token::symbol::RBRACKET),
    skip(token::symbol::ASSIGN), expr_parser);

inline auto name_field_parser = sequence(
    [](auto key, auto, auto value) {
      return ast::name_field{std::move(key), std::move(value)};
    },
    name_parser, skip(token::symbol::ASSIGN), expr_parser);

inline auto value_field_parser = transform(
    expr_parser, [](auto expr) { return ast::value_field{std::move(expr)}; });
}  // namespace lua::parser
