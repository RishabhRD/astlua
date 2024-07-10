#include "parser/combinator.hpp"
#include "ast/ast.hpp"
#include "doctest.hpp"
#include "token/token.hpp"
#include "token/token_info.hpp"

using namespace lua;
using namespace lua::parser;

namespace combinator_test {
namespace __desc {
auto pass(std::string_view desc,
          std::vector<lua::token::token_info> const& tokens, auto f,
          auto const& res, std::size_t len) {
  sub(desc.data()) {
    auto begin = std::begin(tokens);
    auto end = std::end(tokens);
    auto res_opt = f(begin, end);
    req(res_opt.has_value() == true);
    req(res_opt->first == res);
    req(res_opt->second - begin == len);
  }
}

auto fail(std::string_view desc,
          std::vector<lua::token::token_info> const& tokens, auto f) {
  sub(desc.data()) {
    auto begin = std::begin(tokens);
    auto end = std::end(tokens);
    auto res_opt = f(begin, end);
    req(res_opt.has_value() == false);
  }
}
}  // namespace __desc

auto to_token_info(std::vector<token::token_t> const& tokens) {
  std::vector<token::token_info> infos;
  for (auto const& t : tokens) {
    infos.push_back(token::token_info{t, {}});
  }
  return infos;
}

auto pass(std::string_view desc, std::vector<lua::token::token_t> const& tokens,
          auto f, auto const& res, std::size_t len) {
  __desc::pass(desc, to_token_info(tokens), f, res, len);
}

auto fail(std::string_view desc, std::vector<lua::token::token_t> const& tokens,
          auto f) {
  __desc::fail(desc, to_token_info(tokens), f);
}
}  // namespace combinator_test

auto parse_eq = match(token::symbol::EQ, ast::binary_op::EQ);
auto parse_false = match(token::keyword::FALSE, ast::false_t{});
auto parse_true = match(token::keyword::TRUE, ast::true_t{});
auto parse_vararg = match(token::symbol::VARARG, ast::vararg{});

using namespace combinator_test;

test("match") {
  pass("only equal", {token::symbol::EQ}, parse_eq, ast::binary_op::EQ, 1);
  pass("equal with other tokens", {token::symbol::EQ, token::keyword::DO},
       parse_eq, ast::binary_op::EQ, 1);
  fail("false: no element", {}, parse_eq);
  fail("false: wrong element", {token::illegal{}}, parse_eq);
}

test("choice") {
  auto parser = choice<ast::expr>(parse_false, parse_true, parse_vararg);
  pass("only false", {token::keyword::FALSE}, parser, ast::expr{ast::false_t{}},
       1);
  pass("only true", {token::keyword::TRUE}, parser, ast::expr{ast::true_t{}},
       1);
  pass("only vararg", {token::symbol::VARARG}, parser, ast::expr{ast::vararg{}},
       1);
}

test("maybe") {
  auto parser = maybe(parse_false);
  pass("only false", {token::keyword::FALSE}, parser, ast::false_t{}, 1);
  pass("no element", {}, parser, std::nullopt, 0);
  pass("with wrong element", {token::keyword::TRUE}, parser, std::nullopt, 0);
}

test("zero_or_more") {
  auto parser = zero_or_more(parse_false);
  pass("empty", {}, parser, std::vector<ast::false_t>{}, 0);
  pass("0 false", {token::keyword::TRUE}, parser, std::vector<ast::false_t>{},
       0);
  pass("1 false", {token::keyword::FALSE, token::keyword::TRUE}, parser,
       std::vector{ast::false_t{}}, 1);
  pass("2 false",
       {token::keyword::FALSE, token::keyword::FALSE, token::keyword::TRUE},
       parser, std::vector{ast::false_t{}, ast::false_t{}}, 2);
}

test("one_or_more") {
  auto parser = one_or_more(parse_false);
  fail("empty", {}, parser);
  fail("0 false", {token::keyword::TRUE}, parser);
  pass("1 false", {token::keyword::FALSE, token::keyword::TRUE}, parser,
       ast::list_1{ast::false_t{}}, 1);
  pass("2 false",
       {token::keyword::FALSE, token::keyword::FALSE, token::keyword::TRUE},
       parser, ast::list_1{ast::false_t{}, {ast::false_t{}}}, 2);
}

test("sequence") {
  auto eq_false = sequence([](auto, auto) { return ast::false_t(); }, parse_eq,
                           parse_false);
  pass("=false", {token::symbol::EQ, token::keyword::FALSE}, eq_false,
       ast::false_t(), 2);
  fail("false", {token::keyword::FALSE}, eq_false);
  fail("=", {token::symbol::EQ}, eq_false);
  fail("", {}, eq_false);
  fail("something else", {token::symbol::LT}, eq_false);
}
