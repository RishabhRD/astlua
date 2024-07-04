#include <string>
#include "lexer/parse_number.hpp"

int main() {
  std::string str{"0x1P+3"};
  auto res = lua::lexer::parse_hex(std::begin(str), std::end(str));
  if (res.has_value()) {
    std::string r{std::begin(str), *res};
    std::string e{*res, std::end(str)};
    std::string{};
  }
  return 0;
}
