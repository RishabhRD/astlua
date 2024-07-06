#include <string_view>
#include "doctest.hpp"
#include "lexer/enum_rep.hpp"

test("to_keyword") {
  using namespace lua::lexer::tokens;
  auto pass = [](std::string_view token, auto keyword) {
    sub(token.data()) {
      std::string_view rep{token};
      req(lua::lexer::to_keyword(begin(rep), end(rep)) == keyword);
    }
  };
  pass("and", keyword::AND);
  pass("break", keyword::BREAK);
  pass("do", keyword::DO);
  pass("elseif", keyword::ELSEIF);
  pass("else", keyword::ELSE);
  pass("end", keyword::END);
  pass("false", keyword::FALSE);
  pass("for", keyword::FOR);
  pass("function", keyword::FUNCTION);
  pass("if", keyword::IF);
  pass("in", keyword::IN);
  pass("local", keyword::LOCAL);
  pass("nil", keyword::NIL);
  pass("not", keyword::NOT);
  pass("or", keyword::OR);
  pass("repeat", keyword::REPEAT);
  pass("return", keyword::RETURN);
  pass("then", keyword::THEN);
  pass("true", keyword::TRUE);
  pass("until", keyword::UNTIL);
  pass("while", keyword::WHILE);
}

test("to_symbol") {
  using namespace lua::lexer::tokens;
  auto pass = [](std::string_view token, auto res) {
    sub(token.data()) {
      std::string_view rep{token};
      req(lua::lexer::to_symbol(begin(rep), end(rep)) == res);
    }
  };

  pass("+", symbol::PLUS);
  pass("-", symbol::MINUS);
  pass("*", symbol::PROD);
  pass("/", symbol::DIV);
  pass("%", symbol::MOD);
  pass("^", symbol::EXP);
  pass("#", symbol::LEN);
  pass("==", symbol::EQ);
  pass("~=", symbol::NE);
  pass("<=", symbol::LTE);
  pass(">=", symbol::GTE);
  pass("<", symbol::LT);
  pass(">", symbol::GT);
  pass("=", symbol::ASSIGN);
  pass("(", symbol::LPAREN);
  pass(")", symbol::RPAREN);
  pass("{", symbol::LBRACE);
  pass("}", symbol::RBRACE);
  pass("[", symbol::LBRACKET);
  pass("]", symbol::RBRACKET);
  pass(";", symbol::SEMICOLON);
  pass(":", symbol::COLON);
  pass(",", symbol::COMMA);
  pass("...", symbol::VARARG);
  pass("..", symbol::CONCAT);
  pass(".", symbol::MEMBER);
}
