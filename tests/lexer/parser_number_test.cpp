#include "doctest.hpp"
#include "lexer/parse_number.hpp"
#include <string_view>

using namespace astlua::__parse_number;

test("parse positive integer without e") {
  auto res = parse_number("123");
  req(res.has_value());
  req(*res == 3);
}

test("parse positive integer with e") {
  auto res = parse_number("123e9");
  req(res.has_value());
  req(*res == 5);
}
