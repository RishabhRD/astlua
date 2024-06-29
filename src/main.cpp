#include "lexer/parse_number.hpp"
#include <string>
int main() {
  std::string num{"123e9"};
  auto res = astlua::__parse_number::parse_number(num);
  if (res.has_value()) {
  }
  return 0;
}
