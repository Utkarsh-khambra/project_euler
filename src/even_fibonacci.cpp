#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <numeric>
#include <range/v3/view.hpp>

auto sum_even_fibo(std::uint64_t limit) {
  std::uint64_t a = 1;
  std::uint64_t b = 1;
  std::uint64_t sum = 0;
  while (b < limit) {
    b = a + b;
    a = b - a;
    if (a % 2 == 0)
      sum += a;
  }
  return sum;
}

auto sum_even_fib_range(std::uint64_t limit) {

  auto is_even = [](std::uint64_t a) noexcept { return a % 2 == 0; };

  auto even_fibonacci_series = ranges::views::common(
      ranges::views::generate([p = std::pair{1ul, 1ul}]() mutable {
        auto [a, b] = p;
        p = {b, a + b};
        return a;
      }) |
      ranges::views::take_while(
          [=](std::uint64_t a) noexcept { return a < limit; }) |
      ranges::views::filter(is_even));

  return std::accumulate(even_fibonacci_series.begin(),
                         even_fibonacci_series.end(), 0ul);
}

TEST_CASE("2 Even Fibonacci numbers", "") {
  const std::uint64_t lim = 4000000;
  REQUIRE(sum_even_fib_range(lim) == 4613732);
}
