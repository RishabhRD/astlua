#include <string>
#include "lexer/parse_keyword.hpp"

int main() {
  std::string str{"local t = 4;"};
  auto res = lua::lexer::parse_keyword(std::begin(str), std::end(str));
  if (res.has_value()) {}
  return 0;
}
