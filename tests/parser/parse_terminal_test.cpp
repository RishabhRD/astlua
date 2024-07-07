#include "parser/parse_terminal.hpp"
#include "ast/ast.hpp"
#include "doctest.hpp"
#include "functional.hpp"
#include "token/token.hpp"

using namespace lua;
using namespace lua::parser;
using namespace lua::ast;

namespace parse_teminal_test {
template <typename F, typename Res>
auto pass(std::string_view desc, std::vector<lua::token::token_t> const& tokens,
          F f, Res const& res) {
  sub(desc.data()) {
    auto begin = std::begin(tokens);
    auto end = std::end(tokens);
    auto res_opt = f(begin, end);
    req(res_opt.has_value() == true);
    auto [val, next] = *res_opt;
    req(val == res);
    req(next - begin == 1);
  }
}

template <typename F>
auto fail(std::string_view desc, std::vector<lua::token::token_t> const& tokens,
          F f) {
  sub(desc.data()) {
    auto begin = std::begin(tokens);
    auto end = std::end(tokens);
    auto res_opt = f(begin, end);
    req(res_opt.has_value() == false);
  }
}
}  // namespace parse_teminal_test

using namespace parse_teminal_test;

test("parse_false") {
  pass("only false", {token::keyword::FALSE}, lift(parse_false), false_t{});
  pass("false with other tokens", {token::keyword::FALSE, token::symbol::EQ},
       lift(parse_false), false_t{});
  fail("false: no element", {}, lift(parse_false));
  fail("false: wrong element", {token::illegal{}}, lift(parse_false));
}

test("parse_true") {
  pass("only true", {token::keyword::TRUE}, lift(parse_true), true_t{});
  pass("true with other tokens", {token::keyword::TRUE, token::symbol::EQ},
       lift(parse_true), true_t{});
  fail("true: no element", {}, lift(parse_true));
  fail("true: wrong element", {token::illegal{}}, lift(parse_true));
}

test("parse_nil") {
  pass("only nil", {token::keyword::NIL}, lift(parse_nil), nil{});
  pass("nil with other tokens", {token::keyword::NIL, token::symbol::EQ},
       lift(parse_nil), nil{});
  fail("nil: no element", {}, lift(parse_nil));
  fail("nil: wrong element", {token::illegal{}}, lift(parse_nil));
}

test("parse_vararg") {
  pass("only vararg", {token::symbol::VARARG}, lift(parse_vararg), vararg{});
  pass("vararg with other tokens", {token::symbol::VARARG, token::symbol::EQ},
       lift(parse_vararg), vararg{});
  fail("vararg: no element", {}, lift(parse_vararg));
  fail("vararg: wrong element", {token::illegal{}}, lift(parse_vararg));
}

test("parse_binary_op") {
  auto exec = [](binary_op op, token::token_t token) {
    pass("only binary_op", {token}, lift(parse_binary_op), op);
    pass("binary_op with other tokens", {token, token::symbol::EQ},
         lift(parse_binary_op), op);
    fail("binary_op: no element", {}, lift(parse_binary_op));
    fail("binary_op: wrong element", {token::illegal{}}, lift(parse_binary_op));
  };
  exec(binary_op::PLUS, token::symbol::PLUS);
  exec(binary_op::MINUS, token::symbol::MINUS);
  exec(binary_op::PROD, token::symbol::PROD);
  exec(binary_op::DIV, token::symbol::DIV);
  exec(binary_op::EXP, token::symbol::EXP);
  exec(binary_op::MOD, token::symbol::MOD);
  exec(binary_op::CONCAT, token::symbol::CONCAT);
  exec(binary_op::LT, token::symbol::LT);
  exec(binary_op::LTE, token::symbol::LTE);
  exec(binary_op::GT, token::symbol::GT);
  exec(binary_op::GTE, token::symbol::GTE);
  exec(binary_op::EQ, token::symbol::EQ);
  exec(binary_op::NE, token::symbol::NE);
  exec(binary_op::AND, token::keyword::AND);
  exec(binary_op::OR, token::keyword::OR);
}

test("parse_unary_op") {
  auto exec = [](unary_op op, token::token_t token) {
    pass("only unary_op", {token}, lift(parse_unary_op), op);
    pass("unary_op with other tokens", {token, token::symbol::EQ},
         lift(parse_unary_op), op);
    fail("unary_op: no element", {}, lift(parse_unary_op));
    fail("unary_op: wrong element", {token::illegal{}}, lift(parse_unary_op));
  };
  exec(unary_op::MINUS, token::symbol::MINUS);
  exec(unary_op::NOT, token::keyword::NOT);
  exec(unary_op::LEN, token::symbol::LEN);
}

test("parse_field_sep") {
  auto exec = [](field_sep op, token::token_t token) {
    pass("only field_sep", {token}, lift(parse_field_sep), op);
    pass("field_sep with other tokens", {token, token::symbol::EQ},
         lift(parse_field_sep), op);
    fail("field_sep: no element", {}, lift(parse_field_sep));
    fail("field_sep: wrong element", {token::illegal{}}, lift(parse_field_sep));
  };
  exec(field_sep::COMMA, token::symbol::COMMA);
  exec(field_sep::SEMICOLON, token::symbol::SEMICOLON);
}
