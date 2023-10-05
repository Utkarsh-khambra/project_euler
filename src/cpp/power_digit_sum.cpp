#include <array>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <cmath>
#include <fmt/core.h>

// Why double can store 2^1000 correctly
TEST_CASE("16 Power digit sum", "") {
  auto res =
      std::pow(static_cast<long double>(2), static_cast<long double>(1000));
  std::array<char, 10000> num;
  auto [ptr, ec] = std::to_chars(num.data(), num.data() + 10000, res,
                                 std::chars_format::fixed);
  int sum = 0;
  for (auto i = num.data(); i != ptr; ++i)
    sum += *i - '0';
  REQUIRE(1366 == sum);
}
