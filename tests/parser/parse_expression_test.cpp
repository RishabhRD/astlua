#include "ast/ast.hpp"
#include "doctest.hpp"
#include "parser/parse_expresssion.hpp"
#include "token/token.hpp"
#include "token/token_info.hpp"

using namespace lua;
using namespace lua::parser;

namespace parse_expression_test {
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
}  // namespace parse_expression_test

using namespace parse_expression_test;

test("name_parser") {
  pass("name", {token::identifier("hello")}, name_parser, "hello", 1);
  pass("name something", {token::identifier("hello"), token::keyword::DO},
       name_parser, "hello", 1);
  fail("", {}, name_parser);
  fail("do", {token::keyword::DO}, name_parser);
}

test(",name parser") {
  pass(",name", {token::symbol::COMMA, token::identifier("hello")},
       comma_name_parser, "hello", 2);
  pass(",name something",
       {token::symbol::COMMA, token::identifier("hello"), token::keyword::DO},
       comma_name_parser, "hello", 2);
  fail(",", {token::symbol::COMMA}, comma_name_parser);
  fail("name", {token::identifier("hello")}, comma_name_parser);
  fail("", {}, comma_name_parser);
  fail("do", {token::keyword::DO}, comma_name_parser);
}

test("name list parser") {
  pass("name", {token::identifier("hello")}, name_list_parser,
       ast::list_1{std::string{"hello"}}, 1);
  pass("name,", {token::identifier("hello"), token::symbol::COMMA},
       name_list_parser, ast::list_1{std::string{"hello"}}, 1);
  pass("name,name",
       {token::identifier("hello"), token::symbol::COMMA,
        token::identifier("world")},
       name_list_parser,
       ast::list_1{std::string{"hello"}, std::vector<std::string>{"world"}}, 3);
  pass("name,name,",
       {token::identifier("hello"), token::symbol::COMMA,
        token::identifier("world"), token::symbol::COMMA},
       name_list_parser,
       ast::list_1{std::string{"hello"}, std::vector<std::string>{"world"}}, 3);
  pass(
      "name,name,name",
      {token::identifier("hello"), token::symbol::COMMA,
       token::identifier("world"), token::symbol::COMMA,
       token::identifier("!")},
      name_list_parser,
      ast::list_1{std::string{"hello"}, std::vector<std::string>{"world", "!"}},
      5);
}

test("number_parser") {
  pass("name", {token::number("1")}, number_parser, ast::number{"1"}, 1);
  pass("name something", {token::number("1"), token::keyword::DO},
       number_parser, ast::number{"1"}, 1);
  fail("", {}, number_parser);
  fail("do", {token::keyword::DO}, number_parser);
}

test("string_parser") {
  pass("name", {token::string("1")}, string_parser, ast::string{"1"}, 1);
  pass("name something", {token::string("1"), token::keyword::DO},
       string_parser, ast::string{"1"}, 1);
  fail("", {}, string_parser);
  fail("do", {token::keyword::DO}, string_parser);
}

test("nil_parser") {
  pass("name", {token::keyword::NIL}, nil_parser, ast::nil{}, 1);
  pass("name something", {token::keyword::NIL, token::keyword::DO}, nil_parser,
       ast::nil{}, 1);
  fail("", {}, nil_parser);
  fail("do", {token::keyword::DO}, nil_parser);
}

test("true_parser") {
  pass("name", {token::keyword::TRUE}, true_parser, ast::true_t{}, 1);
  pass("name something", {token::keyword::TRUE, token::keyword::DO},
       true_parser, ast::true_t{}, 1);
  fail("", {}, true_parser);
  fail("do", {token::keyword::DO}, true_parser);
}

test("false_parser") {
  pass("name", {token::keyword::FALSE}, false_parser, ast::false_t{}, 1);
  pass("name something", {token::keyword::FALSE, token::keyword::DO},
       false_parser, ast::false_t{}, 1);
  fail("", {}, false_parser);
  fail("do", {token::keyword::DO}, false_parser);
}
