#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <ranges>
#include <vector>

static auto sieve_of_eratosthens(size_t n) {
  std::vector<bool> a(n, true);
  size_t sqrt_n = std::sqrt(n);
  for (size_t i = 2; i < sqrt_n; ++i) {
    if (a[i]) {
      auto j = i * i;
      for (; j < n; j += i) {
        a[j] = false;
      }
    }
  }
  std::vector<std::uint64_t> ret;
  for (size_t i = 2; i < n; ++i) {
    if (a[i])
      ret.push_back(i);
  }
  return ret;
}

TEST_CASE("3 Largest prime factor", "") {
  auto p = sieve_of_eratosthens(std::sqrt(600851475143));

  auto prime_factors = p | std::views::filter([](auto d) noexcept {
                         return 600851475143 % d == 0;
                       });
  REQUIRE(6857 == *std::ranges::max_element(prime_factors));
}
