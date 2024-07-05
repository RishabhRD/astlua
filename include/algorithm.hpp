#pragma once

#include <concepts>
#include <iterator>

namespace nostd {
template <std::input_iterator Iter, std::predicate<Iter> Pred>
constexpr Iter find_if_iter(Iter begin, Iter end, Pred pred) {
  while (begin != end) {
    if (pred(begin))
      return begin;
    ++begin;
  }
  return end;
}
}  // namespace nostd
