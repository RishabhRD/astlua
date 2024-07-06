#include "lexer/lexer.hpp"
#include "doctest.hpp"
#include "token.hpp"

test("extract_token") {
  auto pass = [](std::string_view str, lua::token_t expected_token,
                 std::size_t expected_len) {
    sub(str.data()) {
      auto [token, next_iter] =
          lua::lexer::extract_token(std::begin(str), std::end(str));

      req(token == expected_token);
      auto const len = std::distance(std::begin(str), next_iter);
      req(len == expected_len);
    }
  };

  using namespace lua::tokens;
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
