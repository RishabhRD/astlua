#pragma once

#include <iterator>
#include <optional>
#include <vector>
#include "ast/ast.hpp"
#include "parser/parsed.hpp"
#include "token/token.hpp"

namespace lua::parser {
template <typename Ret>
inline auto match(token::token_t token, Ret ast_node) {
  return [token_ = std::move(token),
          ast_node_ = std::move(ast_node)]<std::forward_iterator Iter>(
             Iter begin, Iter end) -> parsed<Ret, Iter> {
    if (begin != end && *begin == token_)
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
  return [first_ = std::move(first),
          ... rest_ = std::move(rest)]<std::forward_iterator Iter>(
             Iter begin, Iter end) -> parsed<NodeType, Iter> {
    auto res = first_(begin, end);
    if (res)
      return std::pair{static_cast<NodeType>(res->first), res->second};
    return choice<NodeType>(rest_...)(begin, end);
  };
}

template <typename Parser>
inline auto maybe(Parser parser) {
  return [parser_ = std::move(parser)]<typename Iter>(Iter begin, Iter end)
             -> parsed<std::optional<parse_result_t<Parser, Iter>>, Iter> {
    auto res = parser_(begin, end);
    if (res)
      return std::pair{std::optional{res->first}, res->second};
    return std::pair{std::nullopt, begin};
  };
}

template <typename Parser>
inline auto zero_or_more(Parser parser) {
  return [parser_ = std::move(parser)]<typename Iter>(Iter begin, Iter end)
             -> parsed<std::vector<parse_result_t<Parser, Iter>>, Iter> {
    std::vector<parse_result_t<Parser, Iter>> vec;
    while (true) {
      auto res = parser_(begin, end);
      if (!res)
        break;
      begin = res->second;
      vec.push_back(std::move(res->first));
    }
    return std::pair{std::move(vec), begin};
  };
}

template <typename Parser>
inline auto one_or_more(Parser parser) {
  return [parser_ = std::move(parser)]<typename Iter>(Iter begin, Iter end)
             -> parsed<ast::list_1<parse_result_t<Parser, Iter>>, Iter> {
    auto first_res = parser_(begin, end);
    if (!first_res)
      return std::nullopt;
    begin = first_res->second;
    std::vector<parse_result_t<Parser, Iter>> vec;
    while (true) {
      auto res = parser_(begin, end);
      if (!res)
        break;
      begin = res->second;
      vec.push_back(std::move(res->first));
    }
    return std::pair{ast::list_1{std::move(first_res->first), std::move(vec)},
                     begin};
  };
}
}  // namespace lua::parser
