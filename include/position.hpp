#pragma once

#include <cstddef>
namespace lua {
struct position {
  std::size_t line;
  std::size_t col;

  friend bool operator==(position const &, position const &) = default;
};
} // namespace lua
