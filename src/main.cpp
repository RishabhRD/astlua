#include <string>
#include <vector>
#include "lexer/skip_non_tokens.hpp"

int main() {
  std::string const str{"--[[]]f"};
  auto begin = std::begin(str);
  auto end = std::end(str);
  auto itr = lua::lexer::skip_non_tokens(begin, end);
  if (itr == std::end(str)) {}
}
