#pragma once

#include <optional>
#include <utility>

namespace lua::parser {
template <typename T, typename Iter>
using parsed = std::optional<std::pair<T, Iter>>;
}
