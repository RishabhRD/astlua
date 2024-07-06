#include <string_view>
#include "doctest.hpp"
#include "lexer/parse_string.hpp"

namespace parse_short_string_test {
auto pass(std::string_view str, std::size_t len) {
  sub(str.data()) {
    auto res = lua::lexer::parse_short_string(std::begin(str), std::end(str));
    req(res.has_value());
    req(*res - std::begin(str) == len);
  }
}

auto fail(std::string_view str) {
  sub(str.data()) {
    auto res = lua::lexer::parse_short_string(std::begin(str), std::end(str));
    req(res.has_value() == false);
  }
}
}  // namespace parse_short_string_test

using namespace parse_short_string_test;

test("pass: single quoted string with no escape sequence") {
  pass("'h'", 3);
  pass("''", 2);
  pass("'h'  ", 3);
  pass("'h '  ", 4);
  pass("'h 'g", 4);
  pass("'h '\n", 4);
}

test("fail: single quoted string with no escape sequence") {
  fail("f''");
  fail("f");
  fail("'\n'");
  fail("'\r'");
  fail("'h\n'");
  fail("'\nh'");
  fail("'h\ne'");
  fail("'h\re'");
}

test("pass: single quoted string with single escape sequence") {
  pass("'h\\a'", 5);
  pass("'h\\b'", 5);
  pass("'h\\f'", 5);
  pass("'h\\n'", 5);
  pass("'h\\r'", 5);
  pass("'h\\t'", 5);
  pass("'h\\v'", 5);
  pass("'h\\\\'", 5);
  pass("'h\\\"'", 5);
  pass("'h\\''", 5);
}

test("pass: single quoted string with hex escape sequence") {
  pass("'h\\x1'", 6);
  pass("'h\\xa'", 6);
  pass("'h\\x11'", 7);
  pass("'h\\xff'", 7);
}

test("fail: single quoted string with hex escape sequence") {
  fail("'h\\x'");
  fail("'h\\xg'");
  fail("'h\\xgh'");
}

test("pass: single quoted string with unicode escape sequence") {
  pass("'h\\u{1f}f'", 10);
  pass("'\\u{f}'", 7);
  pass("'\\u{ff}'", 8);
  pass("'\\u{fff}'", 9);
  pass("'\\u{fff1}'", 10);
  pass("'\\u{ffff}'", 10);
  pass("'\\u{0ffff}'", 11);
  pass("'\\u{1ffff}'", 11);
  pass("'\\u{00ffff}'", 12);
  pass("'\\u{01ffff}'", 12);
  pass("'\\u{10ffff}'", 12);
}

test("fail: single quoted string with unicode escape sequence") {
  fail("'\\u{}'");
  fail("'\\u{g}'");
  fail("'\\u{fg}'");
  fail("'\\u{ffg}'");
  fail("'\\u{fffg}'");
  fail("'\\u{gfff}'");
  fail("'\\u{ffffg}'");
  fail("'\\u{11ffff}'");
  fail("'\\u{fffffff}'");
  fail("'\\u{'");
  fail("'\\u'");
  fail("'\\u}'");
  fail("'\\uf}'");
  fail("'\\uf'");
}

test("pass: double quoted string with no escape sequence") {
  pass("\"h\"", 3);
  pass("\"\"", 2);
  pass("\"h\"  ", 3);
  pass("\"h \"  ", 4);
  pass("\"h \"g", 4);
  pass("\"h \"\n", 4);
}

test("fail: double quoted string with no escape sequence") {
  fail("f\"\"");
  fail("f");
  fail("\"\n\"");
  fail("\"\r\"");
  fail("\"h\n\"");
  fail("\"\nh\"");
  fail("\"h\ne\"");
  fail("\"h\re\"");
}

test("pass: double quoted string with double escape sequence") {
  pass("\"h\\a\"", 5);
  pass("\"h\\b\"", 5);
  pass("\"h\\f\"", 5);
  pass("\"h\\n\"", 5);
  pass("\"h\\r\"", 5);
  pass("\"h\\t\"", 5);
  pass("\"h\\v\"", 5);
  pass("\"h\\\\\"", 5);
  pass("\"h\\\"\"", 5);
  pass("\"h\\'\"", 5);
}

test("pass: double quoted string with hex escape sequence") {
  pass("\"h\\x1\"", 6);
  pass("\"h\\xa\"", 6);
  pass("\"h\\x11\"", 7);
  pass("\"h\\xff\"", 7);
}

test("fail: double quoted string with hex escape sequence") {
  fail("\"h\\x\"");
  fail("\"h\\xg\"");
  fail("\"h\\xgh\"");
}

test("pass: double quoted string with unicode escape sequence") {
  pass("\"h\\u{1f}f\"", 10);
  pass("\"\\u{f}\"", 7);
  pass("\"\\u{ff}\"", 8);
  pass("\"\\u{fff}\"", 9);
  pass("\"\\u{fff1}\"", 10);
  pass("\"\\u{ffff}\"", 10);
  pass("\"\\u{0ffff}\"", 11);
  pass("\"\\u{1ffff}\"", 11);
  pass("\"\\u{00ffff}\"", 12);
  pass("\"\\u{01ffff}\"", 12);
  pass("\"\\u{10ffff}\"", 12);
}

test("fail: double quoted string with unicode escape sequence") {
  fail("\"\\u{}\"");
  fail("\"\\u{g}\"");
  fail("\"\\u{fg}\"");
  fail("\"\\u{ffg}\"");
  fail("\"\\u{fffg}\"");
  fail("\"\\u{gfff}\"");
  fail("\"\\u{ffffg}\"");
  fail("\"\\u{11ffff}\"");
  fail("\"\\u{fffffff}\"");
  fail("\"\\u{\"");
  fail("\"\\u\"");
  fail("\"\\u}\"");
  fail("\"\\uf}\"");
  fail("\"\\uf\"");
}

test("pass: include both single quote and double quote") {
  pass("'\"'", 3);
  pass("\"'\"", 3);
}

test("fail: confilict between ") {
  fail("\"hello'");
  fail("'hello\"");
}
