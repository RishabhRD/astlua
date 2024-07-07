#pragma once

#include <ostream>
#include "position.hpp"
#include "token/token.hpp"

namespace lua {
namespace token {
struct token_info {
  token_t token;
  position_t position;

  friend bool operator==(token_info const&, token_info const&) = default;

  friend std::ostream& operator<<(std::ostream& os, token_info const& info) {
    os << "{token: " << info.token << ", position: " << info.position << "}";
    return os;
  }
};
}  // namespace token
}  // namespace lua
