#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <vector>
static auto sieve_of_eratosthens(size_t n) {
  std::vector<bool> a(n, true);
  size_t sqrt_n = std::sqrt(n);
  for (size_t i = 2; i < sqrt_n; ++i) {
    if (a.at(i)) {
      auto j = i * i;
      for (; j < n; j += i) {
        a.at(j) = false;
      }
    }
  }
  std::vector<std::uint32_t> ret;
  for (size_t i = 2; i < n; ++i) {
    if (a[i])
      ret.push_back(i);
  }
  return ret;
}

TEST_CASE("7 10001st prime", "") {
  auto p = sieve_of_eratosthens(114320);
  REQUIRE(p[10000] == 104743);
}
