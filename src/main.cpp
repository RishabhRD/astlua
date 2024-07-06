#include <lexer/token_info.hpp>
#include <string>
#include <vector>
#include "lexer/skip_non_tokens.hpp"

int main() {
  std::string str{"--[[]]f"};
  auto itr = lua::lexer::skip_non_tokens(std::begin(str), std::end(str));
  if (itr == std::end(str)) {}
}
