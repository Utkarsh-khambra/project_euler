#include <algorithm>
#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <functional>
#include <numeric>
#include <ranges>
#include <set>
#include <utility>

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

// return all factors without multiplying them
auto C_factors(int n, int k) {
  assert(k < n);
  auto big = k > n - k ? k : n - k;
  auto small = n - big;
  std::vector<int> denom(static_cast<size_t>(small));
  std::ranges::copy(std::views::iota(1, small + 1), denom.begin());
  auto ret = std::views::iota(big + 1, n + 1) |
             std::views::transform([&](int a) noexcept {
               for (auto &i : denom) {
                 if (auto gcd = std::gcd(i, a); gcd != 1) {
                   a = a / gcd;
                   i = i / gcd;
                   if (a == 1)
                     break;
                 }
               }
               return a;
             });
  auto p = std::vector<std::uint64_t>(ret.begin(), ret.end());
  p.push_back(1);
  return p;
}

std::uint64_t C(int n, int k) {
  auto ret = C_factors(n, k);
  return std::reduce(ret.begin(), ret.end(), std::uint64_t{1},
                     std::multiplies<>());
}

// Slower solution
void solution_1(int limit) {
  std::set<std::uint64_t> binomials;
  for (auto i : std::views::iota(1, limit)) {
    for (auto j : std::views::iota(0, std::ceil((i + 1) / 2.0))) {
      binomials.insert(C(i, j));
    }
  }
  auto max = *std::ranges::max_element(binomials);
  auto primes = sieve_of_eratosthens(std::sqrt(max));
  std::ranges::transform(primes, primes.begin(),
                         [](auto i) noexcept { return i * i; });
  auto squarefree_binom =
      binomials | std::views::filter([&primes = std::as_const(primes)](
                                         std::uint64_t i) noexcept -> bool {
        auto large_prime = std::ranges::upper_bound(primes, i);
        for (auto itr = primes.begin(); itr != large_prime; ++itr) {
          if (i % *itr == 0)
            return false;
        }
        return true;
      });
  fmt::print("{}\n", squarefree_binom);
  auto sum = std::reduce(squarefree_binom.begin(), squarefree_binom.end(),
                         std::uint64_t{0});
  fmt::print("Solution 1 {}\n", sum);
}

bool divisble_by_square(const std::vector<uint32_t> &primes,
                        const std::vector<uint64_t> &vals) {
  for (auto i : primes) {
    int count = 0;
    for (auto j : vals) {
      if (j % (i * i) == 0)
        return true;
      if (j % i == 0)
        ++count;
      if (count > 1)
        return true;
    }
  }
  return false;
}

auto solution_2(int limit) {
  auto primes = sieve_of_eratosthens(51);
  std::set<std::uint64_t> binomials;
  std::uint64_t sum = 0;
  for (auto i : std::views::iota(1, limit)) {
    for (auto j : std::views::iota(0, std::ceil((i + 1) / 2.0))) {
      auto temp = C_factors(i, j);
      if (!divisble_by_square(primes, temp)) {
        binomials.insert(std::reduce(temp.begin(), temp.end(), std::uint64_t{1},
                                     std::multiplies<>()));
      }
    }
  }

  return std::reduce(binomials.begin(), binomials.end(), std::uint64_t{0});
}

TEST_CASE("203 Squarefree Binomial Coefficients", "") {
  REQUIRE(solution_2(51) == 34029210557338);
}
