#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <ranges>
#include <unordered_set>
#include <vector>

// ╔══════════════════════════════════════════════════════════╗
// ║                     Remaing problems                     ║
// ║    - We will get maximum length cycle only on primes     ║
// ╚══════════════════════════════════════════════════════════╝

static auto sieve_of_eratosthens(size_t n) {
  std::vector<bool> a(n + 1, true);
  size_t sqrt_n = std::sqrt(n) + 1;
  for (size_t i = 2; i < sqrt_n; ++i) {
    if (a[i]) {
      auto j = i * i;
      for (; j <= n; j += i) {
        a[j] = false;
      }
    }
  }
  std::vector<std::uint64_t> ret;
  for (size_t i = 2; i <= n; ++i) {
    {
      if (a[i])
        ret.push_back(i);
    }
  }
  return ret;
}

auto reciprocal(int n) {
  std::vector<int> digits;
  std::unordered_set<int> ns;
  ns.insert(1);
  int numerator = 10;
  ns.insert(10);
  while (numerator < n) {
    numerator *= 10;
    digits.push_back(0);
  }
  ns.insert(numerator);
  while (true) {
    digits.push_back(numerator / n);
    numerator = (numerator % n);
    if (ns.contains(numerator) || numerator == 0)
      break;
    ns.insert(numerator);
    numerator *= 10;
    while (numerator < n) {
      numerator *= 10;
      digits.push_back(0);
    }
  }
  return digits;
}

// Prove that primes will produce max length

TEST_CASE("26 Reciprocal cycles", "") {
  size_t max = 0;
  auto p = sieve_of_eratosthens(1000);
  for (auto i : p) {
    auto temp = reciprocal(i).size();
    // fmt::print("Done for {} {}\n", i, temp);
    if (temp > max)
      max = i;
  }
  REQUIRE(max == 983);
  // fmt::print("{}\n", max);
}
