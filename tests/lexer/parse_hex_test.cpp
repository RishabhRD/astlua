#include <string_view>
#include "doctest.hpp"
#include "lexer/parse_number.hpp"

using namespace lua::lexer;

namespace parse_hex_test {
auto pass(std::string_view number, std::size_t len) {
  auto res = parse_hex(number);
  req(res.has_value());
  req(*res == len);
}

auto fail(std::string_view number) {
  auto res = parse_hex(number);
  req(res.has_value() == false);
}
}  // namespace parse_hex_test

using namespace parse_hex_test;

test("test_pass_hex: only integers") {
  pass("0x1abcdefABCDEF", 15);
  pass("0X0", 3);
}

test("test_pass_hex: including only .") {
  pass("0x1.", 4);
  pass("0x1.1", 5);
  pass("0x.a", 4);
}

test("test_pass_hex: including only p") {
  pass("0xapf", 5);
  pass("0x1Pf", 5);
  pass("0xap+f", 6);
  pass("0x1P+3", 6);
}

test("test_pass_hex: including p and .") {
  pass("0x.fp9", 6);
  pass("0xf.3p-9", 8);
  pass("0x.ep+9", 7);
  pass("0x1.ep+9", 8);
  pass("0x.ep-3", 7);
  pass("0x1.ep-5", 8);
}

test("test_pass_hex: ends with valid token") {
  pass("0x123 ", 5);
  pass("0x0;", 3);
  pass("0x0+", 3);
  pass("0x0-", 3);
  pass("0x0*", 3);
  pass("0x0=", 3);
  pass("0x0>", 3);
  pass("0x0<", 3);
  pass("0x0^", 3);
  pass("0x0%", 3);
  pass("0x0,", 3);
  pass("0x0\n", 3);
}

test("test_fail_hex: bad input start") {
  fail("f");
  fail("1xf123");
  fail("");
}

test("test_fail_hex: no digit") {
  fail("0x");
  fail("0xp9");
  fail("0x.");
  fail("0x.p3");
  fail("0x.pf");
}

test("test_fail_hex: bad input at end") {
  fail("0x123g");
  fail("0x123.p3g");
  fail("0x123.pe");
}

test("test_fail_hex: p and . conflict") {
  fail("0x123p.2");
}
