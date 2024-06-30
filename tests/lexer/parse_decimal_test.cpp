#include "doctest.hpp"
#include "lexer/parse_number.hpp"
#include <string_view>

using namespace astlua::lexer;

auto pass(std::string_view number, std::size_t len) {
  auto res = parse_decimal(number);
  req(res.has_value());
  req(*res == len);
}

auto fail(std::string_view number) {
  auto res = parse_decimal(number);
  req(res.has_value() == false);
}

test("PASS: only integers") {
  pass("123", 3);
  pass("0", 1);
}

test("PASS: including only .") {
  pass(".", 1);
  pass("1.", 2);
  pass("1.2", 3);
}

test("PASS: including only e") {
  pass("1e9", 3);
  pass("1e-9", 4);
  pass("12e+9", 5);
}

test("PASS: including e and .") {
  pass("1.3e9", 5);
  pass(".3e-9", 5);
  pass(".e+9", 4);
  pass(".e-9", 4);
  pass("1.e-9", 5);
}

test("PASS: ends with valid token") {
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

test("FAIL: bad input start") {
  fail("f");
  fail("f123");
  fail("");
}

test("FAIL: bad input at end") {
  fail("123g");
  fail("123.eg");
  fail("123.3e");
  fail("123d");
}

test("FAIL: e and . conflict") { fail("123e.2"); }
