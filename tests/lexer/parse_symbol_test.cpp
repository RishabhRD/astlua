#include "lexer/parse_symbol.hpp"
#include <ranges>
#include <string_view>
#include "doctest.hpp"

namespace parse_symbol_test {
auto pass(std::string_view str, std::size_t len) {
  auto itr = lua::lexer::parse_symbol(std::begin(str), std::end(str));
  req(itr.has_value());
  req((*itr - std::begin(str)) == len);
}

auto pass(std::string_view symbol) {
  pass(symbol, symbol.size());
}

auto fail(std::string_view str) {
  auto itr = lua::lexer::parse_symbol(std::begin(str), std::end(str));
  req(itr.has_value() == false);
}
}  // namespace parse_symbol_test

using namespace parse_symbol_test;

test("parse_symbol test") {
  pass(">= 4", 2);
  pass("== 4", 2);
  pass("= = 4", 1);
  fail("@");
  fail("");

  auto symbols =
      std::views::transform(lua::lexer::ordered_symbol_string_rep,
                            [](auto const& rep) { return rep.second; });
  std::ranges::for_each(symbols, [](auto s) { pass(s); });
}
