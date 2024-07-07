#include "parser/parse_terminal.hpp"
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
