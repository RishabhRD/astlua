#include "doctest.hpp"
#include "lexer/parse_number.hpp"
#include <string_view>

using namespace lua::lexer;

auto test_pass_hex(std::string_view number, std::size_t len) {
  auto res = parse_hex(number);
  req(res.has_value());
  req(*res == len);
}

auto test_fail_hex(std::string_view number) {
  auto res = parse_hex(number);
  req(res.has_value() == false);
}

test("test_pass_hex: only integers") {
  test_pass_hex("0x1abcdefABCDEF", 15);
  test_pass_hex("0X0", 3);
}

test("test_pass_hex: including only .") {
  test_pass_hex("0x1.", 4);
  test_pass_hex("0x1.1", 5);
  test_pass_hex("0x.a", 4);
}

test("test_pass_hex: including only p") {
  test_pass_hex("0xapf", 5);
  test_pass_hex("0x1Pf", 5);
  test_pass_hex("0xap+f", 6);
  test_pass_hex("0x1P+3", 6);
}

test("test_pass_hex: including p and .") {
  test_pass_hex("0x.fp9", 6);
  test_pass_hex("0xf.3p-9", 8);
  test_pass_hex("0x.ep+9", 7);
  test_pass_hex("0x1.ep+9", 8);
  test_pass_hex("0x.ep-3", 7);
  test_pass_hex("0x1.ep-5", 8);
}

test("test_pass_hex: ends with valid token") {
  test_pass_hex("0x123 ", 5);
  test_pass_hex("0x0;", 3);
  test_pass_hex("0x0+", 3);
  test_pass_hex("0x0-", 3);
  test_pass_hex("0x0*", 3);
  test_pass_hex("0x0=", 3);
  test_pass_hex("0x0>", 3);
  test_pass_hex("0x0<", 3);
  test_pass_hex("0x0^", 3);
  test_pass_hex("0x0%", 3);
  test_pass_hex("0x0,", 3);
  test_pass_hex("0x0\n", 3);
}

test("test_fail_hex: bad input start") {
  test_fail_hex("f");
  test_fail_hex("1xf123");
  test_fail_hex("");
}

test("test_fail_hex: no digit") {
  test_fail_hex("0x");
  test_fail_hex("0xp9");
  test_fail_hex("0x.");
  test_fail_hex("0x.p3");
  test_fail_hex("0x.pf");
}

test("test_fail_hex: bad input at end") {
  test_fail_hex("0x123g");
  test_fail_hex("0x123.p3g");
  test_fail_hex("0x123.pe");
}

test("test_fail_hex: p and . conflict") { test_fail_hex("0x123p.2"); }
