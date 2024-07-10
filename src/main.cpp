#include "ast/ast.hpp"
#include "parser/combinator.hpp"
#include "parser/parse_expresssion.hpp"
#include "token/token.hpp"
#include "token/token_info.hpp"

using namespace lua;
using namespace lua::parser;

int main() {
  std::vector<token::token_info> tokens{
      {token::symbol::COMMA, {}},
      {token::identifier{"hello"}, {}},
  };
  if (comma_name_parser(std::begin(tokens), std::end(tokens))) {
    return 0;
  }
}
