#include "lexer/parse_keyword.hpp"
#include <ranges>
#include <string_view>
#include "doctest.hpp"

namespace parse_keyword_test {
auto pass(std::string_view str, std::size_t len) {
  sub(str.data()) {
    auto itr = lua::lexer::parse_keyword(std::begin(str), std::end(str));
    req(itr.has_value());
    req((*itr - std::begin(str)) == len);
  }
}

auto pass(std::string_view keyword) {
  pass(keyword, keyword.size());
}

auto fail(std::string_view str) {
  sub(str.data()) {
    auto itr = lua::lexer::parse_keyword(std::begin(str), std::end(str));
    req(itr.has_value() == false);
  }
}
}  // namespace parse_keyword_test

using namespace parse_keyword_test;

test("parse_keyword test") {
  pass("local t = 4", 5);
  pass("elseif\nsomething", 6);
  pass("else something", 4);
  pass("local--something", 5);
  pass("local.", 5);
  pass("local--[[something]]", 5);
  pass("local--\n[[something", 5);
  fail("elseifs ");
  fail("elseif9 ");
  fail("el ");
  fail("el");
  fail("");
  fail("<");

  auto keywords =
      std::views::transform(lua::token::ordered_keyword_string_rep,
                            [](auto const& rep) { return rep.second; });
  std::ranges::for_each(keywords, [](auto s) { pass(s); });
}
