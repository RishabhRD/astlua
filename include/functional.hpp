#pragma once

#include <concepts>
#include <memory>

namespace nostd {
#define lift(func)                                                       \
  [](auto&&... __args__) noexcept(                                       \
      noexcept(func(std::forward<decltype(__args__)>(__args__)...)))     \
      -> decltype(func(std::forward<decltype(__args__)>(__args__)...)) { \
    return func(std::forward<decltype(__args__)>(__args__)...);          \
  }

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

template <std::equality_comparable T>
bool is_equal(std::unique_ptr<T> const& a, std::unique_ptr<T> const& b) {
  if (a == nullptr && b == nullptr)
    return true;
  if (a != nullptr && b != nullptr)
    return *(a.get()) == *(b.get());
  return false;
}
}  // namespace nostd
