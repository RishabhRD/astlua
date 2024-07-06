#include "lexer/token.hpp"
#include "lexer/enum_rep.hpp"

namespace lua {
namespace lexer {
namespace tokens {
std::ostream& operator<<(std::ostream& os, lua::lexer::tokens::keyword k) {
  os << "keyword{";
  os << std::ranges::find_if(lua::lexer::ordered_keyword_string_rep,
                             [k](auto const& rep) { return rep.first == k; })
            ->second;
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, lua::lexer::tokens::symbol k) {
  os << "symbol{";
  os << std::ranges::find_if(lua::lexer::ordered_symbol_string_rep,
                             [k](auto const& rep) { return rep.first == k; })
            ->second;
  os << "}";
  return os;
}

}  // namespace tokens

std::ostream& operator<<(std::ostream& os, token_t const& token_var) {
  std::visit(nostd::overload{
                 [&os](tokens::keyword const& x) { os << x; },
                 [&os](tokens::symbol const& x) { os << x; },
                 [&os](tokens::string const& x) { os << x; },
                 [&os](tokens::identifier const& x) { os << x; },
                 [&os](tokens::number const& x) { os << x; },
                 [&os](tokens::illegal const& x) { os << x; },
             },
             token_var);
  return os;
}
}  // namespace lexer
}  // namespace lua
