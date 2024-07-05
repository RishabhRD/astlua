#include <string_view>
#include "doctest.hpp"
#include "lexer/parse_string.hpp"

namespace parse_long_string_test {
auto pass(std::string_view str, std::size_t len) {
  sub(str.data()) {
    auto res = lua::lexer::parse_long_string(std::begin(str), std::end(str));
    req(res.has_value());
    req(*res - std::begin(str) == len);
  }
}

auto fail(std::string_view str) {
  sub(str.data()) {
    auto res = lua::lexer::parse_long_string(std::begin(str), std::end(str));
    req(res.has_value() == false);
  }
}
}  // namespace parse_long_string_test

using namespace parse_long_string_test;

test("pass cases which doesn't include =") {
  pass("[[hello]]", 9);
  pass("[[hello] ]]", 11);
  pass("[[hello\n] ]]", 12);
  pass("[[]]", 4);
  pass("[[   ]]", 7);
  pass("[[hello]]g", 9);
  pass("[[hello] ]] ", 11);
  pass("[[hello\n] ]][] ", 12);
  pass("[[]]   ", 4);
  pass("[[   ]]]]", 7);
}

test("pass cases which includes =") {
  pass("[=[hello]=]", 11);
  pass("[==[hello]==]", 13);
  pass("[==[hello\n]==]", 14);
  pass("[=[]=]", 6);
  pass("[==[]==]", 8);
  pass("[=[]]=]", 7);
  pass("[==[]=]==]", 10);
  pass("[=[hello]=]]]", 11);
  pass("[==[hello]==][[", 13);
  pass("[==[hello\n]==]  ", 14);
  pass("[=[]=]fjfj", 6);
  pass("[==[]==]23", 8);
  pass("[=[]]=]=", 7);
  pass("[==[]=]==]==", 10);
}

test("fail cases") {
  fail("f[[]]");
  fail("f[==[]=]");
  fail("[==[]=]");
  fail("[=[]==]");
  fail("[[] ");
  fail("[=[]=");
  fail("[=[]=   ");
  fail("[]");
  fail("[===]");
}
