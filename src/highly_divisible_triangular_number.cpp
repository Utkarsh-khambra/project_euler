#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>
int total_divisors(std::uint64_t n) {
  auto sqrt_n = std::sqrt(n);
  auto result = 0;
  std::uint64_t i = 2;
  while (i < sqrt_n) {
    if (n % i == 0)
      result += 2;
    ++i;
  }
  if (n % i == 0) {
    ++result;
    if (i * i != n)
      ++result;
  }
  return result + 2;
}

// Instead of finding divisors of large triangle number we find divisors of
// smaller n/2 and n+1
int total_divisors_triangle_number(std::uint64_t a, std::uint64_t b) {
  return total_divisors(a) * total_divisors(b);
}
TEST_CASE("12 Highly divisible triangular number", "") {

  std::uint64_t triangle_num = 28;
  for (std::uint64_t i = 8; true; ++i) {
    triangle_num = triangle_num + i;
    int a, b;
    if (i % 2 == 0) {
      a = i / 2;
      b = i + 1;
    } else {
      a = (i + 1) / 2;
      b = i;
    }
    if (total_divisors_triangle_number(a, b) > 500) {
      REQUIRE(triangle_num == 76576500);
      break;
    }
  }
}
