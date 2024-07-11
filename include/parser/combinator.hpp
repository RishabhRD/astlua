#pragma once

#include <functional>
#include <iterator>
#include <optional>
#include <type_traits>
#include <vector>
#include "ast/ast.hpp"
#include "parser/parsed.hpp"
#include "token/token.hpp"

namespace lua::parser {
template <typename Ret>
inline auto always(Ret ret_node) {
  return [node = std::move(ret_node)]<std::input_iterator Iter>(
             Iter begin, Iter) -> parsed<Ret, Iter> {
    return std::pair{node, begin};
  };
}

template <typename Ret>
inline auto pure(Ret ret_node) {
  return always(std::move(ret_node));
}

template <typename If, typename Then>
inline auto match_if_then(If pred, Then then) {
  return
      [pred_ = std::move(pred),
       then_ = std::move(then)]<std::input_iterator Iter>(
          Iter begin, Iter end) -> parsed<decltype(then(begin->token)), Iter> {
        if (begin != end && pred_(begin->token)) {
          auto val = begin->token;
          return std::pair{then_(std::move(val)), ++begin};
        }
        return std::nullopt;
      };
}

template <typename Ret>
inline auto match(token::token_t token, Ret ast_node) {
  return [token_ = std::move(token),
          ast_node_ = std::move(ast_node)]<std::input_iterator Iter>(
             Iter begin, Iter end) -> parsed<Ret, Iter> {
    if (begin != end && begin->token == token_)
      return std::pair{ast_node_, ++begin};
    return std::nullopt;
  };
}

inline auto skip(token::token_t token) {
  return match(std::move(token), std::monostate{});
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
      return std::pair{NodeType{res->first}, res->second};
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

namespace __seq_details {
constexpr auto papply = []<typename F, typename... Args>(F f, Args... args) {
  if constexpr (std::invocable<F, Args...>) {
    return std::move(f)(std::move(args)...);
  } else {
    return std::bind_front(std::move(f), std::move(args)...);
  }
};

template <typename F, typename P, typename I>
using result_type =
    parsed<std::invoke_result_t<decltype(papply), parse_result_t<F, I>,
                                parse_result_t<P, I>>,
           I>;

template <typename P1, typename P2>
inline auto parser_apply(P1 p1, P2 p2) {
  return [p1_ = std::move(p1), p2_ = std::move(p2)]<typename Iter>(
             Iter begin, Iter end) -> result_type<P1, P2, Iter> {
    if (auto p1_r = p1_(begin, end)) {
      if (auto p2_r = p2_(p1_r->second, end)) {
        return std::pair{papply(std::move(p1_r->first), std::move(p2_r->first)),
                         p2_r->second};
      }
      return std::nullopt;
    }
    return std::nullopt;
  };
}

template <typename FunctionParser, typename Parser, typename... Parsers>
inline auto parser_apply(FunctionParser pf, Parser p, Parsers... ps) {
  return parser_apply(parser_apply(std::move(pf), std::move(p)),
                      std::move(ps)...);
}
}  // namespace __seq_details

template <typename Function, typename... Parser>
inline auto sequence(Function f, Parser... p) {
  return __seq_details::parser_apply(always(std::move(f)), std::move(p)...);
}
}  // namespace lua::parser
