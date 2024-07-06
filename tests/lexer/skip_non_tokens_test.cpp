#include "lexer/skip_non_tokens.hpp"
#include "doctest.hpp"

namespace skip_non_tokens {
auto pass(std::string_view str, std::size_t len) {
  sub(str.data()) {
    req(lua::lexer::skip_non_tokens(std::begin(str), std::end(str)) -
            std::begin(str) ==
        len);
  }
}
}  // namespace skip_non_tokens

using namespace skip_non_tokens;

test("only whitespace") {
  pass("", 0);
  pass("hello", 0);
  pass(" ", 1);
  pass("\n", 1);
  pass("\t", 1);
  pass("\r", 1);
  pass(" \r\n\tf", 4);
  pass(" \r\n\tf ", 4);
}

test("only single line comment") {
  pass("--\n", 3);
  pass("--", 2);
  pass("--\nd", 3);
  pass("--h\nd", 4);
  pass("--h\n--m\nd", 8);
}

test("only multiline comments") {
  pass("--[[]]", 6);
  pass("--[[]]f", 6);
  pass("--[[]]--[[]]", 12);
  pass("--[[h]]--[[]]", 13);
  pass("--[[h]]--[[j]]", 14);
  pass("--[=[]=]", 8);
  pass("--[=[]=]--[==[]==]", 18);
  pass("--[=[h]=]--[=[]=]", 17);
  pass("--[===[h]===]--[[j]]", 20);
  pass(" --t =\n\r--[[hello]] t = [[hello]]\r\n  @", 20);
}

test("all non tokens mixed") {
  pass(" --h\n--[[y]] \n--h\n ", 19);
}
