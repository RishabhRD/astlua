#pragma once

#include <iterator>
#include "parser/parsed.hpp"
#include "token/token.hpp"

namespace lua::parser {
template <typename Ret>
inline auto match(token::token_t token, Ret ast_node) {
  return [token_ = std::move(token),
          ast_node_ = std::move(ast_node)]<std::input_iterator Iter>(
             Iter begin, Iter end) -> parsed<Ret, Iter> {
    if (begin == end)
      return std::nullopt;
    if (*begin == token_)
      return std::pair{ast_node_, ++begin};
    return std::nullopt;
  };
}
}  // namespace lua::parser
