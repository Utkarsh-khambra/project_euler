#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <numeric>

static bool check_overflow_on_mult(std::uint64_t a, std::uint64_t b) {
  return a > std::numeric_limits<std::uint64_t>::max() / b;
}

std::uint64_t collatz_chain_length(std::uint64_t n) {
  std::uint64_t length = 0;
  auto p = n;
  while (p != 1) {
    ++length;
    if (p % 2 == 0)
      p /= 2;
    else
      p = 3 * p + 1;
  }
  return length;
}
TEST_CASE("14 Longest Collatz sequence", "") {
  std::uint64_t max = 0;
  int number = 0;
  for (int i = 1; i < 1000000; ++i) {
    /* fmt::print("Doing for {}\n", i); */
    if (auto temp = collatz_chain_length(i); max < temp) {
      max = temp;
      number = i;
    }
  }
  REQUIRE(number == 837799);
}
