#pragma once

#include <cstddef>

namespace lua {
struct position_t {
  std::size_t line;
  std::size_t col;

  friend bool operator==(position_t const&, position_t const&) = default;
};
}  // namespace lua
