#include <string_view>
#include "doctest.hpp"
#include "lexer/commons.hpp"

using namespace lua::lexer;

namespace parse_prefix_test {
auto pass(std::string_view str, std::string_view prefix,
          std::size_t expected_dist) {
  auto res = parse_prefix(std::begin(str), std::end(prefix), prefix);
  req(res.has_value());
  auto actual_dist = static_cast<std::size_t>((*res) - std::begin(str));
  req(actual_dist == expected_dist);
}

auto fail(std::string_view str, std::string_view prefix) {
  auto res = parse_prefix(std::begin(str), std::end(prefix), prefix);
  req(res.has_value() == false);
}
}  // namespace parse_prefix_test

using namespace parse_prefix_test;

test("parse prefix") {
  pass("hello", "he", 2);
  pass("he", "he", 2);
  fail("xyz", "he");
  fail("xy", "he");
  fail("h", "he");
  fail("", "he");
}
