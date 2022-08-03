#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>

TEST_CASE("120 Square remainders", "") {
  long sum = 0;
  for (long i = 3; i <= 1000; ++i) {
    long rem = 2 * i;
    auto j = i;
    if (i % 2 == 0)
      j = j - 1;
    long n = std::floor(j / 2);
    rem *= n;
    assert(rem < i * i);
    sum += rem;
  }
  REQUIRE(sum == 333082500);
}
