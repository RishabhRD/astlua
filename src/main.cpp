#include "lexer/parse_number.hpp"
#include <string>
int main() {
  std::string num{"0x.a"};
  auto res = astlua::lexer::parse_hex(num);
  if (res.has_value()) {
  }
  return 0;
}
