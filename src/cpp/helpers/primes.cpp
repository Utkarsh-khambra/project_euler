#include <algorithm>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

static auto sieve_of_eratosthens(size_t n) {
  std::vector<bool> a(n, true);
  size_t sqrt_n = std::sqrt(n);
  for (size_t i = 2; i <= sqrt_n; ++i) {
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

template <typename F>
constexpr void cool_lex(size_t s, size_t t, F &&f) noexcept {
  std::vector<bool> b(s + t + 1, true);
  for (auto i : std::views::iota(t + 1, s + t + 1))
    b[i] = false;
  size_t x = t;
  size_t y = t;
  f(b | std::views::drop(1));
  while (x < s + t) {
    b[x] = 0;
    b[y] = 1;
    b[1] = b[x + 1];
    b[x + 1] = 1;
    x = x + 1 - (x - 1) * b[2] * (1 - b[1]);
    y = b[1] * y + 1;
    f(b | std::views::drop(1));
  }
}

template <std::ranges::range R, typename F>
  requires(std::invocable<F, std::ranges::range_value_t<R>> &&
           std::is_same_v<
               void, std::invoke_result_t<F, std::ranges::range_value_t<R>>>)
constexpr void for_each_combination(R &&r, size_t m, F &&f) noexcept {
  auto func = [&](auto &&filter) {
    for (auto [ind, i] : filter | std::views::enumerate) {
      if (i)
        f(*std::ranges::next(std::ranges::begin(r), ind));
    }
  };
  cool_lex(r.size() - m, m, func);
}

template <std::ranges::range R, typename F>
  requires(std::invocable<F, std::ranges::range_value_t<R>> &&
           !std::is_same_v<
               void, std::invoke_result_t<F, std::ranges::range_value_t<R>>>)
constexpr void for_each_combination(R &&r, size_t m, F &&f) noexcept {
  auto func = [&](auto &&filter) {
    for (auto [ind, i] : filter | std::views::enumerate) {
      if (i) {
        auto &val = *std::ranges::next(std::ranges::begin(r), ind);
        val = f(val);
      }
    }
  };
  cool_lex(r.size() - m, m, func);
}

template <std::ranges::range R, typename F>
  requires(std::invocable<F, R>)
std::vector<std::invoke_result_t<F, R>>
reduce_combinations(R &&r, size_t m, F &&reduce) noexcept {
  using return_t = std::invoke_result_t<F, R>;
  std::vector<return_t> ret;
  // std::cout << "Fuck";
  auto func = [&](auto filter) {
    size_t index = 0;
    auto combination =
        r | std::views::filter([&](auto const &i) {
          return *std::ranges::next(std::ranges::begin(filter), index++);
        });
    for (auto i : combination)
      index = 0;
    ret.push_back(reduce(combination));
  };
  cool_lex(r.size() - m, m, func);
  return ret;
}

// template <typename sieve>
// requires(std::invocable<sieve, size_t>)
inline size_t legendre_formula(size_t n) noexcept {
  assert(n > 20);
  auto primes = sieve_of_eratosthens(std::ceil(std::sqrt(n)));
  auto mult_reduce = [](auto r) -> size_t {
    size_t val = 1;
    for (auto i : r) {
      std::cout << "Fuck " << i << " ";
      val *= i;
    }
    std::cout << "\n";
    return val;
  };
  auto sum = primes.size() + n;
  for (auto i : std::views::iota(size_t{1}, primes.size() + 1)) {
    auto f = reduce_combinations(primes, i, mult_reduce);
    for (auto &j : f) {
      j = static_cast<size_t>(std::floor(static_cast<double>(n) / j));
      std::cout << j << " ";
      sum += j * -1 * (i % 2);
    }
    std::cout << "\n ";
  }
  return sum;
}

inline size_t meissel_lehmer(size_t n) noexcept {}

int main() {
  std::vector<char> s{'a', 'b', 'c', 'd', 'f'};
  auto id = [](char c) { std::cout << c; };
  // for_each_combination(s, 3, id);
  // cool_lex(1, 2);
  auto pi_n = legendre_formula(30);
  std::cout << "pi_n " << pi_n;
  // for_each_combination(s, 5, id);

  // auto j = sieve_of_eratosthens(std::floor(std::sqrt(30)));
  // for (auto i : j)
  //   std::cout << i << "\n";
}
