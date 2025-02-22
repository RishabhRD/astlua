#include "parser/parser.hpp"
#include "ast/ast.hpp"
#include "doctest.hpp"
#include "token/token.hpp"
#include "token/token_info.hpp"

using namespace lua;
using namespace lua::parser;

namespace parse_expression_test {
namespace __desc {
auto pass(std::string_view desc,
          std::vector<lua::token::token_info> const& tokens, auto f,
          auto const& expected, std::size_t len) {
  sub(desc.data()) {
    auto begin = std::begin(tokens);
    auto end = std::end(tokens);
    auto res_opt = f(begin, end);
    req(res_opt.has_value() == true);
    req(res_opt->first == expected);
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

auto pass(std::string_view desc, std::vector<lua::token::token_t> tokens,
          auto f, auto const& res, std::size_t len) {
  __desc::pass(desc, to_token_info(tokens), f, res, len);
  tokens.push_back(token::illegal());
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

template <typename Node>
auto test_list_parser(auto parser, auto token, Node const& node) {
  pass("token", {token}, parser, ast::list_1{node}, 1);
  pass("token,", {token, token::symbol::COMMA}, parser, ast::list_1{node}, 1);
  pass("token,token", {token, token::symbol::COMMA, token}, parser,
       ast::list_1{node, {node}}, 3);
  pass("token,token,",
       {token, token::symbol::COMMA, token, token::symbol::COMMA}, parser,
       ast::list_1{node, {node}}, 3);
  pass("token,token,token",
       {token, token::symbol::COMMA, token, token::symbol::COMMA, token},
       parser, ast::list_1{node, {node, node}}, 5);
}

test("name list parser") {
  test_list_parser(name_list_parser, token::identifier("hello"),
                   std::string{"hello"});
}

test("exp list parser") {
  test_list_parser(expr_list_parser, token::keyword::FALSE,
                   ast::expr(ast::false_t()));
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

test("break_stat_parser") {
  pass("name", {token::keyword::BREAK}, break_stat_parser, ast::break_stat{},
       1);
  pass("name something", {token::keyword::BREAK, token::keyword::DO},
       break_stat_parser, ast::break_stat{}, 1);
  fail("", {}, break_stat_parser);
  fail("do", {token::keyword::DO}, break_stat_parser);
}

test("fn_name_parser") {
  pass("x", {token::identifier{"x"}}, fn_name_parser, ast::fn_name{"x", {}, {}},
       1);
  pass("x.", {token::identifier{"x"}, token::symbol::MEMBER}, fn_name_parser,
       ast::fn_name{"x", {}, {}}, 1);
  pass("x.y",
       {token::identifier{"x"}, token::symbol::MEMBER, token::identifier{"y"}},
       fn_name_parser, ast::fn_name{"x", {"y"}, {}}, 3);
  pass("x.y.",
       {token::identifier{"x"}, token::symbol::MEMBER, token::identifier{"y"},
        token::symbol::MEMBER},
       fn_name_parser, ast::fn_name{"x", {"y"}, {}}, 3);
  pass("x.y.z",
       {token::identifier{"x"}, token::symbol::MEMBER, token::identifier{"y"},
        token::symbol::MEMBER, token::identifier{"z"}},
       fn_name_parser, ast::fn_name{"x", {"y", "z"}, {}}, 5);
  pass("x.y.z:",
       {token::identifier{"x"}, token::symbol::MEMBER, token::identifier{"y"},
        token::symbol::MEMBER, token::identifier{"z"}, token::symbol::COLON},
       fn_name_parser, ast::fn_name{"x", {"y", "z"}, {}}, 5);
  pass("x.y.z:w",
       {token::identifier{"x"}, token::symbol::MEMBER, token::identifier{"y"},
        token::symbol::MEMBER, token::identifier{"z"}, token::symbol::COLON,
        token::identifier{"w"}},
       fn_name_parser, ast::fn_name{"x", {"y", "z"}, {"w"}}, 7);
  fail("", {}, fn_name_parser);
  fail("illegal", {token::illegal{}}, fn_name_parser);
}

test("field_sep_parser") {
  pass(",", {token::symbol::COMMA}, field_sep_parser, std::monostate(), 1);
  pass(";", {token::symbol::SEMICOLON}, field_sep_parser, std::monostate(), 1);
  fail("", {}, field_sep_parser);
  fail("do", {token::keyword::DO}, field_sep_parser);
}

test("expr_field_parser") {
  pass("['x'] = 2",
       {token::symbol::LBRACKET, token::string("'x'"), token::symbol::RBRACKET,
        token::symbol::ASSIGN, token::number("2")},
       expr_field_parser,
       ast::expr_field(ast::expr(ast::string("'x'")),
                       ast::expr(ast::number("2"))),
       5);
  fail("['x']",
       {token::symbol::LBRACKET, token::string("'x'"), token::symbol::RBRACKET},
       expr_field_parser);
}

test("name_field_parser") {
  pass("x = 2",
       {token::identifier("x"), token::symbol::ASSIGN, token::number("2")},
       name_field_parser, ast::name_field("x", ast::expr(ast::number("2"))), 3);
  fail("x", {token::identifier("x")}, name_field_parser);
}

test("value_field parser") {
  pass("2", {token::number("2")}, value_field_parser,
       ast::value_field(ast::expr(ast::number("2"))), 1);
  fail("illegal", {token::illegal{}}, value_field_parser);
  fail("", {}, value_field_parser);
}

test("field parser") {
  pass("['x'] = 2",
       {token::symbol::LBRACKET, token::string("'x'"), token::symbol::RBRACKET,
        token::symbol::ASSIGN, token::number("2")},
       field_parser,
       ast::field(ast::expr_field(ast::expr(ast::string("'x'")),
                                  ast::expr(ast::number("2")))),
       5);
  pass("x = 2",
       {token::identifier("x"), token::symbol::ASSIGN, token::number("2")},
       field_parser,
       ast::field(ast::name_field("x", ast::expr(ast::number("2")))), 3);
  pass("2", {token::number("2")}, field_parser,
       ast::field(ast::value_field(ast::expr(ast::number("2")))), 1);
  fail("illegal", {token::illegal{}}, value_field_parser);
  fail("", {}, value_field_parser);
}

test("field_list_parser") {
  pass("2", {token::number("2")}, field_list_parser,
       std::vector{ast::field(ast::value_field(ast::expr(ast::number("2"))))},
       1);
  pass("2;", {token::number("2"), token::symbol::SEMICOLON}, field_list_parser,
       std::vector{ast::field(ast::value_field(ast::expr(ast::number("2"))))},
       2);
  pass("2,", {token::number("2"), token::symbol::COMMA}, field_list_parser,
       std::vector{ast::field(ast::value_field(ast::expr(ast::number("2"))))},
       2);
  pass("2;3,4;",
       {token::number("2"), token::symbol::COMMA, token::number("3"),
        token::symbol::COMMA, token::number("4"), token::symbol::SEMICOLON},
       field_list_parser,
       std::vector{
           ast::field(ast::value_field(ast::expr(ast::number("2")))),

           ast::field(ast::value_field(ast::expr(ast::number("3")))),
           ast::field(ast::value_field(ast::expr(ast::number("4")))),
       },
       6);
  fail("illegal", {token::illegal{}}, field_list_parser);
  fail("", {}, field_list_parser);
}

test("table_parser") {
  pass("{2}",
       {token::symbol::LBRACE, token::number("2"), token::symbol::RBRACE},
       table_parser,
       ast::table{std::vector{
           ast::field(ast::value_field(ast::expr(ast::number("2"))))}},
       3);
  pass("{}", {token::symbol::LBRACE, token::symbol::RBRACE}, table_parser,
       ast::table{}, 2);
}

test("var_decl_stat_parser") {
  pass("local x", {token::keyword::LOCAL, token::identifier("x")},
       var_decl_stat_parser, ast::var_decl_stat{{"x"}, {}}, 2);
  pass("local x,y",
       {token::keyword::LOCAL, token::identifier("x"), token::symbol::COMMA,
        token::identifier("y")},
       var_decl_stat_parser, ast::var_decl_stat{{"x", {"y"}}, {}}, 4);
  pass("local x,y=2",
       {token::keyword::LOCAL, token::identifier("x"), token::symbol::COMMA,
        token::identifier("y"), token::symbol::ASSIGN, token::number("2")},
       var_decl_stat_parser,
       ast::var_decl_stat{{"x", {"y"}},
                          ast::list_1<ast::expr>{ast::expr{ast::number{"2"}}}},
       6);
  pass(
      "local x,y=2,3",
      {token::keyword::LOCAL, token::identifier("x"), token::symbol::COMMA,
       token::identifier("y"), token::symbol::ASSIGN, token::number("2"),
       token::symbol::COMMA, token::number("3")},
      var_decl_stat_parser,
      ast::var_decl_stat{{"x", {"y"}},
                         ast::list_1<ast::expr>{ast::expr{ast::number{"2"}},
                                                {ast::expr{ast::number{"3"}}}}},
      8);
}

test("stat_list_parser") {
  pass("", {}, stat_list_parser, std::vector<ast::statement>{}, 0);
  pass("local x", {token::keyword::LOCAL, token::identifier("x")},
       stat_list_parser,
       std::vector<ast::statement>{
           ast::statement{ast::var_decl_stat{{"x"}, {}}},
       },
       2);
  pass("local x local y",
       {token::keyword::LOCAL, token::identifier("x"), token::keyword::LOCAL,
        token::identifier("y")},
       stat_list_parser,
       std::vector<ast::statement>{
           ast::statement{ast::var_decl_stat{{"x"}, {}}},
           ast::statement{ast::var_decl_stat{{"y"}, {}}},
       },
       4);
  pass("local x; local y",
       {token::keyword::LOCAL, token::identifier("x"), token::symbol::SEMICOLON,
        token::keyword::LOCAL, token::identifier("y")},
       stat_list_parser,
       std::vector<ast::statement>{
           ast::statement{ast::var_decl_stat{{"x"}, {}}},
           ast::statement{ast::var_decl_stat{{"y"}, {}}},
       },
       5);
}

test("return_stat_parser") {
  pass("return", {token::keyword::RETURN}, return_stat_parser,
       ast::return_stat{}, 1);
  pass("return 2", {token::keyword::RETURN, token::number{"2"}},
       return_stat_parser, ast::return_stat{{ast::expr(ast::number("2"))}}, 2);
  pass("return 2,3",
       {token::keyword::RETURN, token::number{"2"}, token::symbol::COMMA,
        token::number{"3"}},
       return_stat_parser,
       ast::return_stat{{
           ast::expr(ast::number("2")),
           ast::expr(ast::number("3")),
       }},
       4);
}

test("last_stat_parser") {
  pass("return", {token::keyword::RETURN}, last_stat_parser,
       ast::last_stat{ast::return_stat{}}, 1);
  pass("break", {token::keyword::BREAK}, last_stat_parser,
       ast::last_stat{ast::break_stat{}}, 1);
}

test("param_list_parser") {
  pass("x", {token::identifier{"x"}}, param_list_parser,
       ast::param_list{
           ast::name_param_list{ast::list_1<std::string>{"x"}, false}},
       1);
  pass("x,...",
       {token::identifier{"x"}, token::symbol::COMMA, token::symbol::VARARG},
       param_list_parser,
       ast::param_list{
           ast::name_param_list{ast::list_1<std::string>{"x"}, true}},
       3);
  pass("...", {token::symbol::VARARG}, param_list_parser,
       ast::param_list{ast::vararg_param_list{}}, 1);
}

test("block_parser") {
  pass(";;local x",
       {token::symbol::SEMICOLON, token::symbol::SEMICOLON,
        token::keyword::LOCAL, token::identifier{"x"}},
       block_parser,
       ast::block({ast::statement{ast::var_decl_stat({std::string{"x"}}, {})}},
                  {}),
       4);
  pass(
      ";;local x;",
      {token::symbol::SEMICOLON, token::symbol::SEMICOLON,
       token::keyword::LOCAL, token::identifier{"x"}, token::symbol::SEMICOLON},
      block_parser,
      ast::block({ast::statement{ast::var_decl_stat({std::string{"x"}}, {})}},
                 {}),
      5);
  pass(";;local x;;return",
       {token::symbol::SEMICOLON, token::symbol::SEMICOLON,
        token::keyword::LOCAL, token::identifier{"x"}, token::symbol::SEMICOLON,
        token::symbol::SEMICOLON, token::keyword::RETURN},
       block_parser,
       ast::block({ast::statement{ast::var_decl_stat({std::string{"x"}}, {})}},
                  ast::last_stat{ast::return_stat()}),
       7);
  pass(";;local x;;return;;",
       {token::symbol::SEMICOLON, token::symbol::SEMICOLON,
        token::keyword::LOCAL, token::identifier{"x"}, token::symbol::SEMICOLON,
        token::symbol::SEMICOLON, token::keyword::RETURN,
        token::symbol::SEMICOLON, token::symbol::SEMICOLON},
       block_parser,
       ast::block({ast::statement{ast::var_decl_stat({std::string{"x"}}, {})}},
                  ast::last_stat{ast::return_stat()}),
       9);
}

test("fn_body_parser") {
  pass("()end",
       {token::symbol::LPAREN, token::symbol::RPAREN, token::keyword::END},
       fn_body_parser, ast::fn_body{}, 3);
}

test("fn_parser") {
  pass("function()end",
       {token::keyword::FUNCTION, token::symbol::LPAREN, token::symbol::RPAREN,
        token::keyword::END},
       fn_parser, ast::fn(ast::fn_body{}), 4);
}
