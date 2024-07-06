#include "lexer/lexer.hpp"
#include <iterator>
#include <vector>
#include "doctest.hpp"
#include "lexer/token.hpp"
#include "lexer/token_info.hpp"

test("extract_token") {
  auto pass = [](std::string_view str, lua::lexer::token_t expected_token,
                 std::size_t expected_len) {
    sub(str.data()) {
      auto [token, next_iter] =
          lua::lexer::extract_token(std::begin(str), std::end(str));

      req(token == expected_token);
      auto const len = std::distance(std::begin(str), next_iter);
      req(len == expected_len);
    }
  };

  using namespace lua::lexer::tokens;
  pass("[[hello]]local", string{"[[hello]]"}, 9);
  pass("'hello'local", string{"'hello'"}, 7);
  pass("local[[hello]]", keyword::LOCAL, 5);
  pass("locali[[hello]]", identifier{"locali"}, 6);
  pass(">=[[hello]]", symbol::GTE, 2);
  pass(">=", symbol::GTE, 2);
  pass(">= ", symbol::GTE, 2);
  pass("@", illegal{'@'}, 1);
  pass("@[", illegal{'@'}, 1);
  pass("@ ", illegal{'@'}, 1);
}

test("tokenize") {
  using namespace lua::lexer;
  auto pass = [](std::string_view str,
                 std::vector<token_info> expected_tokens) {
    sub(str.data()) {
      std::vector<token_info> tokens;
      lua::lexer::tokenize(std::begin(str), std::end(str),
                           std::back_inserter(tokens));
      req(tokens == expected_tokens);
    }
  };

  using namespace lua::lexer::tokens;
  pass("local --t =\n\r--[[hello]] t = [[hello]]\r\n  @",
       {
           token_info{keyword::LOCAL, {0, 0}},
           token_info{identifier{"t"}, {2, 12}},
           token_info{symbol::ASSIGN, {2, 14}},
           token_info{string{"[[hello]]"}, {2, 16}},
           token_info{illegal{'@'}, {3, 2}},
       });
}
