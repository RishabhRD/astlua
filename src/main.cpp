#include <string>
#include "lexer/parse_string.hpp"

int main() {
  std::string str{"[=[]]=]="};
  auto res = lua::lexer::parse_long_string(std::begin(str), std::end(str));
  if (res.has_value()) {
    std::string r{std::begin(str), *res};
    std::string e{*res, std::end(str)};
    std::string{};
  }
  return 0;
}
