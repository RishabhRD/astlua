#include "lexer/parse_identifier.hpp"
#include <string_view>
#include "doctest.hpp"

namespace parse_identifier_test {
auto pass(std::string_view str, std::size_t len) {
  sub(str.data()) {
    auto itr = lua::lexer::parse_identifier(std::begin(str), std::end(str));
    req(itr.has_value());
    req((*itr - std::begin(str)) == len);
  }
}

auto fail(std::string_view str) {
  sub(str.data()) {
    auto itr = lua::lexer::parse_identifier(std::begin(str), std::end(str));
    req(itr.has_value() == false);
  }
}
}  // namespace parse_identifier_test

using namespace parse_identifier_test;

test("parse_identifier test") {
  pass("abc", 3);
  pass("_", 1);
  pass("abc_2", 5);
  pass("_abc_2", 6);
  pass("_abc_", 5);
  pass("abc2", 4);
  fail("2abc2");
  fail(".abc2");
  fail(">abc2");
  fail("!abc2");
}
