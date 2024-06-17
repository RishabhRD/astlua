#pragma once

#include "position.hpp"
#include "token.hpp"
namespace astlua {
struct token_info {
  token token;
  position pos;

  friend bool operator==(token_info const &, token_info const &) = default;
};
} // namespace astlua
