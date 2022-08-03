#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <fmt/core.h>
#include <iterator>

int count_sundays() {
  std::chrono::year start(1901);
  std::chrono::year end(2001);
  std::chrono::month first{1};
  int count = 0;
  for (auto i = start; i != end; ++i) {
    for (unsigned j = 1; j < 13; ++j) {
      std::chrono::month m(j);
      std::chrono::year_month_day temp(i / m / 1);
      std::chrono::weekday wd{std::chrono::sys_days{temp}};
      if (wd == std::chrono::Sunday)
        ++count;
    }
  }
  return count;
}

TEST_CASE("19 Counting Sundays", "") { REQUIRE(count_sundays() == 171); }
