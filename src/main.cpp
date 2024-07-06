#include <string>
#include "lexer/parse_string.hpp"

int main() {
  std::string str{"'h'"};
  auto res = lua::lexer::parse_short_string(std::begin(str), std::end(str));
  if (res.has_value()) {
    std::string r{std::begin(str), *res};
    std::string e{*res, std::end(str)};
    std::string{};
  }
  return 0;
}
