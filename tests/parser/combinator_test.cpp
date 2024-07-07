#include "parser/combinator.hpp"
#include "ast/ast.hpp"
#include "doctest.hpp"
#include "token/token.hpp"

using namespace lua;
using namespace lua::parser;

test("match") {
  auto pass = [](std::string_view desc,
                 std::vector<lua::token::token_t> const& tokens, auto f,
                 auto const& res) {
    sub(desc.data()) {
      auto begin = std::begin(tokens);
      auto end = std::end(tokens);
      auto res_opt = f(begin, end);
      req(res_opt.has_value() == true);
      auto [val, next] = *res_opt;
      req(val == res);
      req(next - begin == 1);
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

  auto parse_eq = match(token::symbol::EQ, ast::binary_op::EQ);
  pass("only equal", {token::symbol::EQ}, parse_eq, ast::binary_op::EQ);
  pass("equal with other tokens", {token::symbol::EQ, token::keyword::DO},
       parse_eq, ast::binary_op::EQ);
  fail("false: no element", {}, parse_eq);
  fail("false: wrong element", {token::illegal{}}, parse_eq);
}
