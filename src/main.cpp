#include <lexer/token_info.hpp>
#include <string>
#include <vector>
#include "lexer/lexer.hpp"

int main() {
  std::string str{"local --t =\n\r--[[hello]] t = [[hello]]\r\n  @"};
  std::vector<lua::lexer::token_info> tokens;
  lua::lexer::tokenize(std::begin(str), std::end(str),
                       std::back_inserter(tokens));
}
