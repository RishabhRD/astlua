#include "lexer/parse_number.hpp"
#include <string>
int main() {
  std::string num{"123e9"};
  auto res = astlua::lexer::parse_decimal(num);
  if (res.has_value()) {
  }
  return 0;
}
