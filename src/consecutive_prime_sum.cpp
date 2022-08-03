#include <bitset>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <numeric>
#include <vector>
constexpr uint s = 1000000;
static std::pair<std::bitset<s>, std::vector<uint>>
sieve_of_eratosthens(size_t n) {
  std::bitset<s> a;
  a.set();
  size_t sqrt_n = std::sqrt(n);
  for (size_t i = 2; i <= sqrt_n; ++i) {
    if (a[i]) {
      auto j = i * i;
      for (; j < n; j += i) {
        a[j] = false;
      }
    }
  }
  std::vector<std::uint32_t> ret;
  for (size_t i = 2; i < n; ++i) {
    if (a[i])
      ret.push_back(i);
  }
  return {a, ret};
}

TEST_CASE("50 Consecutive prime sum", "") {

  uint prime = 0;
  uint max = 0;
  auto [prime_set, primes] = sieve_of_eratosthens(s);
  for (uint i = 1; true; ++i) {
    auto last = primes.end() - i + 1;
    for (auto j = primes.begin(); j != last; ++j) {
      auto sum = std::accumulate(j, j + i, static_cast<uint>(0));
      if (sum < s && prime_set[sum]) {
        if (max < i) {
          max = i;
          prime = sum;
        }
        continue;
      }
      if (sum > s) {
        if (j == primes.begin()) {
          // fmt::print("{} {}\n", max, prime);
          REQUIRE(prime == 997651);
          return;
        }
        break;
      }
    }
  }
  REQUIRE(false);
}
