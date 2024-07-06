#pragma once

#include <cstddef>
#include <ostream>

namespace lua {
struct position_t {
  std::size_t line;
  std::size_t col;

  friend bool operator==(position_t const&, position_t const&) = default;

  friend std::ostream& operator<<(std::ostream& os, position_t const& pos) {
    os << "pos(line: " << pos.line << ", col: " << pos.col << ")";
    return os;
  }
};
}  // namespace lua
