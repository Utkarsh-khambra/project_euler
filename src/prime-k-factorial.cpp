#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <numeric>
#include <ranges>
#include <vector>

static auto sieve_of_eratosthens(size_t n) {
  std::vector<bool> a(n, true);
  size_t sqrt_n = std::sqrt(n) + 1;
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
    {
      if (a[i])
        ret.push_back(i);
    }
  }
  return ret;
}
// From euclid extended algorithm
// https://en.m.wikipedia.org/wiki/Extended_Euclidean_algorithm
std::int64_t modulo_inverse(std::int64_t a, std::int64_t m) {
  auto t = 0ll;
  auto new_t = 1ll;
  auto r = m;
  auto new_r = a;
  while (new_r != 0) {
    auto q = r / new_r;
    auto prev = t;
    t = new_t;
    new_t = prev - q * new_t;
    prev = r;
    r = new_r;
    new_r = prev - q * new_r;
  }
  if (r > 1)
    throw std::runtime_error("The fuck");
  if (t < 0)
    t = t + m;
  return t;
}

// Findes sum((p-i)!)%p for i 1 to 5
std::uint64_t prime_factorial(std::int64_t p) {
  // From wilsons theorem (p-1)!%p = -1
  auto prev = p - 1;
  std::uint64_t sum = static_cast<std::uint64_t>(p - 1);
  for (auto i = 2; i < 6; ++i) {
    auto a_inv = modulo_inverse((p - i + 1), p);
    auto temp = (a_inv * prev) % p;
    sum += std::uint64_t(temp);
    prev = temp;
  }
  return sum % p;
}
static auto segmented_sieve(std::uint64_t n) {
  std::uint64_t sqrt_n = std::sqrt(n);
  auto primes = sieve_of_eratosthens(sqrt_n + 1);
  const auto sqrt_size = primes.size();
  auto l = sqrt_n;
  auto h = l + sqrt_n;
  std::vector<bool> a(sqrt_n, true);
  while (l < n) {
    ++l;
    if (h > n) {
      h = n;
    }
    std::fill_n(a.begin(), h - l + 1 * (h != n), true);
    const auto end = primes.begin() + sqrt_size;
    const auto size = h - l + 1 * (h != n);
    for (auto p = primes.begin(); p != end; ++p) {
      auto start = l - 1 + *p - ((l - 1) % *p);
      for (; start < l + size; start += *p) {
        auto ind = start - l;
        a[start - l] = false;
      }
    }
    for (auto i = 0; i < size; ++i) {
      if (a[i]) {
        primes.push_back(l);
      }
      ++l;
    }

    // sieve(l + 1, h, primes, sqrt_size);
    l = h;
    h += sqrt_n;
  }
  return primes;
}

TEST_CASE("381 (prime-k) factorial", "") {
  const std::int64_t p = 100000000;
  auto primes = segmented_sieve(p);
  auto j = primes |
           std::views::filter([](std::uint64_t i) noexcept { return i >= 5; });
  std::uint64_t sum = 0;
  for (const auto i : j) {
    sum += prime_factorial(i);
  }
  // fmt::print("for 1E{} {}\n", std::log10(p), sum);
  REQUIRE(sum == 139602943319822);
}

// Solves in simple way by finding first the Σ (p-i)!%p using
// repeadtly doing multiplication and taking remainder
/* void slow_solutions() {
  auto mult_mod = [](std::uint64_t a, std::uint64_t b,
                     std::uint64_t m) noexcept -> std::uint64_t {
    std::uint64_t ret = 0;
    a = a % m;
    while (b > 0) {
      if (b & 1)
        ret = (ret + a) % m;
      a = (a * 2) % m;
      b = b / 2;
    }
    return ret % m;
  };
  auto factorial_mod_1 = [=](std::uint64_t f,
                             std::uint64_t m) noexcept -> std::uint64_t {
    if (f < 2)
      return 1;
    auto ret = mult_mod(f, f - 1, m);
    f -= 2;
    for (; f > 1; --f)
      ret = mult_mod(ret, f, m);
    return ret;
  };

  auto prime_factorial_mod = [](std::uint64_t f, std::uint64_t m) noexcept {
    auto ret = f--;
    for (; f > 1; --f) {
      ret = ret * f;
      if (ret > m)
        ret = ret % m;
    }
    return ret;
  };

  auto factorial_mod_2 = [](std::uint64_t f,
                            std::uint64_t m) noexcept -> std::uint64_t {
    auto fact = std::views::iota(1ul, f + 1);
    return std::accumulate(fact.begin(), fact.end(), 1ul,
                           [=](std::uint64_t init, std::uint64_t a) noexcept
                           {
                             return (init * a) % m;
                           });
  };
  auto S = [=](std::uint64_t p) noexcept -> std::uint64_t {
    auto small_mod = factorial_mod_1(p - 5, p);
    std::array<std::uint64_t, 5> facts{factorial_mod_1(p - 5, p), p - 4, p -
    3,
                                       p - 2, p - 1};
    for (size_t i = 1; i < 5; ++i) {
      facts[i] = (facts[i - 1] * facts[i]) % p;
    }
    return std::accumulate(facts.begin(), facts.end(), 0ul) % p;
  };

  const std::uint64_t p = 100;
  auto primes = sieve_of_eratosthens(p);
  fmt::print("Done finding primes\n");
  auto sum_S =
      primes |
      std::views::filter([](std::uint64_t i) noexcept { return i >= 5; }) |
      std::views::transform(S);
  auto result = std::accumulate(sum_S.begin(), sum_S.end(), 0ul);
  fmt::print("{}\n", result);
} */
