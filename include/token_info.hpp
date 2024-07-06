#pragma once

#include "position.hpp"
#include "token.hpp"

namespace lua {
struct token_info {
  token_t token;
  position_t position;

  friend bool operator==(token_info const&, token_info const&) = default;
};
}  // namespace lua
