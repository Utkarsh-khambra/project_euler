#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <vector>
constexpr int limit = 1000000;

std::int64_t nCr(std::int64_t n, std::int64_t r) {
  std::int64_t result = 1;
  std::vector<std::int64_t> remaining_rs;
  for (std::int64_t i = 0; i < r; ++i) {
    if ((n - i) % (i + 1) == 0)
      result *= (n - i) / (i + 1);
    else {
      remaining_rs.push_back(i + 1);
      result *= (n - i);
    }
  }
  for (auto i : remaining_rs)
    result /= i;
  return result;
}
int greater_than_million(std::int64_t n) {
  if (n < 23)
    return -1;
  std::int64_t r = 2;
  std::int64_t result = 0;
  std::int64_t prev_result = 0;
  do {
    ++r;
    result = nCr(n, r);
    if (prev_result > result)
      fmt::print("The fuck\n");
    prev_result = result;
  } while (result < limit && r < n);
  return r;
}

TEST_CASE("53 Combinatoric selections", "") {

  int sum = 0;
  for (auto i = 23; i < 101; ++i) {
    auto r = greater_than_million(i);
    sum += i - 2 * r + 1;
  }
  REQUIRE(sum == 4075);
}
