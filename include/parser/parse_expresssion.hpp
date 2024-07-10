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

inline auto comma_name_parser =
    sequence([](auto, auto name) { return name; },
             match(token::symbol::COMMA, std::monostate{}), name_parser);

inline auto name_list_parser = sequence(
    [](auto name, auto lst) {
      return ast::list_1{std::move(name), std::move(lst)};
    },
    zero_or_more(comma_name_parser));

}  // namespace lua::parser
