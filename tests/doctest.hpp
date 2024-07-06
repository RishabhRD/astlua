#include <doctest/doctest.h>
#include <iterator>
#include <ostream>
#include <sstream>
#include <vector>

#define test TEST_CASE
#define req REQUIRE
#define sub SUBCASE

namespace doctest {
template <typename T>
concept streamable = requires(std::ostream& os, T const& t) { os << t; };

template <streamable T>
struct StringMaker<std::vector<T>> {
  static String convert(const std::vector<T>& vec) {
    std::ostringstream oss;
    oss << "[ ";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(oss, " "));
    oss << "]";
    return oss.str().c_str();
  }
};
}  // namespace doctest
