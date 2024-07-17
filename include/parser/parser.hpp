#pragma once

#include <iterator>
#include <variant>
#include "ast/ast.hpp"
#include "functional.hpp"
#include "parser/combinator.hpp"
#include "token/token.hpp"

namespace lua::parser {
template <typename T>
auto list_1_to_vector(std::optional<ast::list_1<T>> lst) -> std::vector<T> {
  if (lst.has_value()) {
    lst->more.insert(std::begin(lst->more), std::move(lst->first));
    return std::move(lst->more);
  } else {
    return {};
  }
}

// Postcondition:
//  - get 2nd argument for 2 argument function
inline auto get_snd2 = [](auto const&, auto val) {
  return val;
};

template <std::forward_iterator Iter>
inline auto expr_parser_fn(Iter begin, Iter end) -> parsed<ast::expr, Iter>;

inline auto expr_parser = lift(expr_parser_fn);

template <std::forward_iterator Iter>
inline auto stat_parser_fn(Iter begin,
                           Iter end) -> parsed<ast::statement, Iter>;

inline auto stat_parser = lift(stat_parser_fn);

inline auto name_parser = match_if_then(
    [](auto const& name_token) {
      return std::holds_alternative<token::identifier>(name_token);
    },
    [](auto name_token) {
      return std::get<token::identifier>(name_token).value;
    });

inline auto snd(auto sep_parser, auto parser) {
  return sequence(get_snd2, sep_parser, parser);
}

inline auto list_parser(auto sep_parser, auto parser) {
  auto comma_parser = snd(std::move(sep_parser), parser);
  return sequence(
      [](auto val, auto lst) {
        return ast::list_1{std::move(val), std::move(lst)};
      },
      std::move(parser), zero_or_more(std::move(comma_parser)));
}

inline auto name_list_parser =
    list_parser(skip(token::symbol::COMMA), name_parser);

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

inline auto expr_list_parser =
    list_parser(skip(token::symbol::COMMA), expr_parser);

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

inline auto field_parser = choice<ast::field>(
    expr_field_parser, name_field_parser, value_field_parser);

inline auto field_list_parser = sequence(
    [](auto field, auto more_fields, auto) {
      more_fields.insert(std::begin(more_fields), std::move(field));
      return more_fields;
    },
    field_parser,
    zero_or_more(sequence(get_snd2, field_sep_parser, field_parser)),
    maybe(field_sep_parser));

inline auto table_parser = sequence(
    [](auto, auto fields, auto) {
      return fields ? ast::table{std::move(*fields)} : ast::table{};
    },
    skip(token::symbol::LBRACE), maybe(field_list_parser),
    skip(token::symbol::RBRACE));

inline auto var_decl_stat_parser = sequence(
    [](auto, auto names, auto exprs) {
      return ast::var_decl_stat{std::move(names), std::move(exprs)};
    },
    skip(token::keyword::LOCAL), name_list_parser,
    maybe(sequence(get_snd2, skip(token::symbol::ASSIGN), expr_list_parser)));

inline auto stat_list_parser = zero_or_more(
    snd(zero_or_more(skip(token::symbol::SEMICOLON)), stat_parser));

inline auto break_stat_parser = match(token::keyword::BREAK, ast::break_stat{});

inline auto return_stat_parser = sequence(
    [](auto, auto expr) {
      return ast::return_stat{list_1_to_vector(std::move(expr))};
    },
    skip(token::keyword::RETURN), maybe(expr_list_parser));

inline auto last_stat_parser =
    choice<ast::last_stat>(return_stat_parser, break_stat_parser);

inline auto block_parser = sequence(
    [](auto stats, auto const&, auto last_stat, auto const&) {
      return ast::block(std::move(stats), std::move(last_stat));
    },
    stat_list_parser, zero_or_more(skip(token::symbol::SEMICOLON)),
    maybe(last_stat_parser), zero_or_more(skip(token::symbol::SEMICOLON)));

inline auto name_param_list_parser = sequence(
    [](auto name_list, auto vararg) {
      return ast::name_param_list{std::move(name_list), vararg.has_value()};
    },
    name_list_parser,
    maybe(snd(skip(token::symbol::COMMA), skip(token::symbol::VARARG))));

inline auto vararg_param_list_parser = transform(
    skip(token::symbol::VARARG), [](auto) { return ast::vararg_param_list{}; });

inline auto param_list_parser =
    choice<ast::param_list>(name_param_list_parser, vararg_param_list_parser);

inline auto fn_body_parser = sequence(
    [](auto, auto params, auto, auto block, auto) {
      return ast::fn_body{std::move(params), std::move(block)};
    },
    skip(token::symbol::LPAREN), maybe(param_list_parser),
    skip(token::symbol::RPAREN), block_parser, skip(token::keyword::END));

namespace __parser_details {
inline auto expr_parser_impl =
    choice<ast::expr>(number_parser, string_parser, nil_parser, true_parser,
                      false_parser, vararg_parser, table_parser);

inline auto stat_parser_impl = choice<ast::statement>(var_decl_stat_parser);
}  // namespace __parser_details

template <std::forward_iterator Iter>
inline auto expr_parser_fn(Iter begin, Iter end) -> parsed<ast::expr, Iter> {
  return __parser_details::expr_parser_impl(begin, end);
}

template <std::forward_iterator Iter>
inline auto stat_parser_fn(Iter begin,
                           Iter end) -> parsed<ast::statement, Iter> {
  return __parser_details::stat_parser_impl(begin, end);
}
}  // namespace lua::parser
