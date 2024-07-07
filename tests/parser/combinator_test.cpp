#include "parser/combinator.hpp"
#include "ast/ast.hpp"
#include "doctest.hpp"
#include "parser/parse_terminal.hpp"
#include "token/token.hpp"

using namespace lua;
using namespace lua::parser;

namespace combinator_test {
auto pass = [](std::string_view desc,
               std::vector<lua::token::token_t> const& tokens, auto f,
               auto const& res, std::size_t len) {
  sub(desc.data()) {
    auto begin = std::begin(tokens);
    auto end = std::end(tokens);
    auto res_opt = f(begin, end);
    req(res_opt.has_value() == true);
    req(res_opt->first == res);
    req(res_opt->second - begin == len);
  }
};

auto fail = [](std::string_view desc,
               std::vector<lua::token::token_t> const& tokens, auto f) {
  sub(desc.data()) {
    auto begin = std::begin(tokens);
    auto end = std::end(tokens);
    auto res_opt = f(begin, end);
    req(res_opt.has_value() == false);
  }
};
}  // namespace combinator_test

using namespace combinator_test;

test("match") {
  auto parse_eq = match(token::symbol::EQ, ast::binary_op::EQ);
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
