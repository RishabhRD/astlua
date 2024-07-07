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
}  // namespace lua::parser
