#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <numeric>
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

TEST_CASE("10 Summation of primes", "") {
  auto p = sieve_of_eratosthens(1999999);
  REQUIRE(142913828922 == std::reduce(p.begin(), p.end(), std::uint64_t{0},
                                      std::plus<std::uint64_t>()));
}
