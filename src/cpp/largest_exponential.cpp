#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <cmath>
#include <fmt/core.h>
#include <fstream>
#include <string>
#include <vector>

TEST_CASE("99 Largest exponential", "") {
  std::ifstream f("../src/base_exp.txt");
  std::vector<std::pair<std::uint32_t, std::uint32_t>> v;
  for (std::string l; std::getline(f, l);) {
    auto comma_pos = l.find(',');
    std::pair<std::uint32_t, std::uint32_t> a{};
    std::from_chars(l.data(), l.data() + comma_pos, a.first);
    std::from_chars(l.data() + comma_pos + 1, l.data() + l.size(), a.second);
    v.push_back(a);
  }

  auto max = std::max_element(v.begin(), v.end(), [](auto &a, auto &b) -> bool {
    return a.second * std::log10(a.first) < b.second * std::log10(b.first);
  });
  REQUIRE((std::distance(v.begin(), max) + 1) == 709);
}

