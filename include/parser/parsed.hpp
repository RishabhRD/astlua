#pragma once

#include <optional>
#include <utility>

namespace lua::parser {
template <typename T, typename Iter>
using parsed = std::optional<std::pair<T, Iter>>;

template <typename Parser, typename Iter>
using parse_result_t =
    typename std::invoke_result_t<Parser, Iter, Iter>::value_type::first_type;
}  // namespace lua::parser
