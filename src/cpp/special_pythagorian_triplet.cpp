#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <fmt/core.h>

TEST_CASE("9 Special Pythagorean triplet", "") {
  for (std::uint64_t b = 1; b < 999; ++b) {
    auto a = (500'000 - 1000 * b) / (1000 - b);
    auto c = 1000 - a - b;
    if (a * a + b * b == c * c) {
      REQUIRE((a, b, c, a * b * c) == 31875000);
      break;
    }
  }
}
