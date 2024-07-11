#pragma once

#include <variant>
#include "ast/ast.hpp"
#include "parser/combinator.hpp"
#include "token/token.hpp"

namespace lua::parser {
inline auto name_parser = match_if_then(
    [](auto const& name_token) {
      return std::holds_alternative<token::identifier>(name_token);
    },
    [](auto name_token) {
      return std::get<token::identifier>(name_token).value;
    });

inline auto list_parser(auto parser) {
  auto comma_val_parser =
      sequence([](auto, auto val) { return val; },
               match(token::symbol::COMMA, std::monostate{}), parser);

  return sequence(
      [](auto val, auto lst) {
        return ast::list_1{std::move(val), std::move(lst)};
      },
      std::move(parser), zero_or_more(comma_val_parser));
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

inline auto exp_parser =
    choice<ast::expr>(number_parser, string_parser, nil_parser, true_parser,
                      false_parser, vararg_parser);
}  // namespace lua::parser
