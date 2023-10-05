#include <algorithm>
#include <bitset>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <vector>
constexpr uint limit = 100'000;
static std::vector<uint> sieve_of_eratosthens(size_t n) {
  std::bitset<limit> a;
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
  return ret;
}

std::uint32_t rad(std::uint32_t n, const std::vector<uint> &primes) {
  std::uint32_t product = 1;
  auto m = n;
  for (auto i : primes) {
    while (m % i == 0)
      m = m / i;
    if (n != m)
      product *= i;
    n = m;
  }
  if (n != 1)
    product *= n;
  return product;
}

// Can be improved because somehow rad should depend on previous values or gcd

TEST_CASE("124 Ordered radicals", "") {
  auto p = sieve_of_eratosthens(limit);
  std::vector<std::pair<std::uint32_t, std::uint32_t>> rads;
  rads.reserve(limit);
  for (std::uint32_t i = 1; i < limit + 1; ++i) {
    rads.push_back({i, rad(i, p)});
  }
  std::sort(rads.begin(), rads.end(), [](auto a, auto b) {
    if (a.second != b.second)
      return a.second < b.second;
    return a.first < b.first;
  });
  REQUIRE(rads[10'000 - 1].first == 21417);
}
