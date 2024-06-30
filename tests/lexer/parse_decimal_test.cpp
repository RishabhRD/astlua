#include "doctest.hpp"
#include "lexer/parse_number.hpp"
#include <string_view>

using namespace astlua::lexer;

auto test_pass_decimal(std::string_view number, std::size_t len) {
  auto res = parse_decimal(number);
  req(res.has_value());
  req(*res == len);
}

auto test_fail_decimal(std::string_view number) {
  auto res = parse_decimal(number);
  req(res.has_value() == false);
}

test("test_pass_decimal: only integers") {
  test_pass_decimal("123", 3);
  test_pass_decimal("0", 1);
}

test("test_pass_decimal: including only .") {
  test_pass_decimal(".", 1);
  test_pass_decimal(".2", 2);
  test_pass_decimal("1.", 2);
  test_pass_decimal("1.2", 3);
}

test("test_pass_decimal: including only e") {
  test_pass_decimal("1e9", 3);
  test_pass_decimal("1e-9", 4);
  test_pass_decimal("12e+9", 5);
}

test("test_pass_decimal: including e and .") {
  test_pass_decimal("1.3e9", 5);
  test_pass_decimal(".3e-9", 5);
  test_pass_decimal(".e+9", 4);
  test_pass_decimal(".e-9", 4);
  test_pass_decimal("1.e-9", 5);
}

test("test_pass_decimal: ends with valid token") {
  test_pass_decimal("123 ", 3);
  test_pass_decimal("0;", 1);
  test_pass_decimal("0+", 1);
  test_pass_decimal("0-", 1);
  test_pass_decimal("0*", 1);
  test_pass_decimal("0=", 1);
  test_pass_decimal("0>", 1);
  test_pass_decimal("0<", 1);
  test_pass_decimal("0^", 1);
  test_pass_decimal("0%", 1);
  test_pass_decimal("0\n", 1);
}

test("test_fail_decimal: bad input start") {
  test_fail_decimal("f");
  test_fail_decimal("f123");
  test_fail_decimal("");
}

test("test_fail_decimal: bad input at end") {
  test_fail_decimal("123g");
  test_fail_decimal("123.eg");
  test_fail_decimal("123.3e");
  test_fail_decimal("123d");
}

test("test_fail_decimal: e and . conflict") { test_fail_decimal("123e.2"); }
