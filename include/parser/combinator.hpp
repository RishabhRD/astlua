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

template <typename NodeType>
inline auto choice() {
  return []<typename Iter>(Iter, Iter) -> parsed<NodeType, Iter> {
    return std::nullopt;
  };
}

template <typename NodeType, typename FirstParser, typename... RestParsers>
inline auto choice(FirstParser first, RestParsers... rest) {
  return
      [first_ = std::move(first), ... rest_ = std::move(rest)]<typename Iter>(
          Iter begin, Iter end) -> parsed<NodeType, Iter> {
        auto res = first_(begin, end);
        if (res)
          return std::pair{static_cast<NodeType>(res->first), res->second};
        return choice<NodeType>(rest_...)(begin, end);
      };
}
}  // namespace lua::parser
