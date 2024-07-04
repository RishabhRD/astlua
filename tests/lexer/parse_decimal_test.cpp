#include <string_view>
#include "doctest.hpp"
#include "lexer/parse_number.hpp"
#include "lexer/parse_symbol.hpp"

using namespace lua::lexer;

namespace parse_decimal_test {
auto pass(std::string_view number, std::size_t len) {
  auto res = parse_decimal(number);
  req(res.has_value());
  req(*res == len);
}

auto fail(std::string_view number) {
  auto res = parse_decimal(number);
  req(res.has_value() == false);
}
}  // namespace parse_decimal_test

using namespace parse_decimal_test;

test("test_pass_decimal: only integers") {
  pass("123", 3);
  pass("0", 1);
}

test("test_pass_decimal: including only .") {
  pass(".", 1);
  pass(".2", 2);
  pass("1.", 2);
  pass("1.2", 3);
}

test("test_pass_decimal: including only e") {
  pass("1e9", 3);
  pass("1e-9", 4);
  pass("12e+9", 5);
}

test("test_pass_decimal: including e and .") {
  pass("1.3e9", 5);
  pass(".3e-9", 5);
  pass(".e+9", 4);
  pass(".e-9", 4);
  pass("1.e-9", 5);
}

test("test_pass_decimal: ends with valid token") {
  pass("123 ", 3);
  pass("0;", 1);
  pass("0+", 1);
  pass("0-", 1);
  pass("0*", 1);
  pass("0=", 1);
  pass("0>", 1);
  pass("0<", 1);
  pass("0^", 1);
  pass("0%", 1);
  pass("0\n", 1);
}

test("test_fail_decimal: bad input start") {
  fail("f");
  fail("f123");
  fail("");
}

test("test_fail_decimal: bad input at end") {
  fail("123g");
  fail("123.eg");
  fail("123.3e");
  fail("123d");
}

test("test_fail_decimal: e and . conflict") {
  fail("123e.2");
}

test("should support appending any digit") {
  std::ranges::for_each(std::begin(lua::lexer::ordered_symbol_string_rep),
                        std::end(lua::lexer::ordered_symbol_string_rep),
                        [](auto rep) {
                          std::string num{"4"};
                          num += rep.second;
                          if (rep.second[0] != '.')
                            pass(num, 1);
                        });
}
