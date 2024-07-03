#include <string>
#include "lexer/parse_number.hpp"

int main() {
  std::string str{"2>="};
  auto res = lua::lexer::parse_decimal(std::begin(str), std::end(str));
  if (res.has_value()) {}
  return 0;
}
