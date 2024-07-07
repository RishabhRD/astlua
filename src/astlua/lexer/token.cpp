#include "token/token.hpp"
#include "token/enum_rep.hpp"

namespace lua {
namespace token {
std::ostream& operator<<(std::ostream& os, keyword k) {
  os << "keyword{";
  os << std::ranges::find_if(ordered_keyword_string_rep, [k](auto const& rep) {
          return rep.first == k;
        })->second;
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, symbol k) {
  os << "symbol{";
  os << std::ranges::find_if(ordered_symbol_string_rep, [k](auto const& rep) {
          return rep.first == k;
        })->second;
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, token_t const& token_var) {
  std::visit([&os](auto const& x) { os << x; }, token_var);
  return os;
}
}  // namespace token
}  // namespace lua
