#include <algorithm>
#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <fmt/core.h>
#include <numeric>
#include <set>
#include <unordered_map>
#include <vector>

// Make it faster

template <std::signed_integral T> class ratio {
public:
  ratio() = delete;
  explicit ratio(T num, T denom) noexcept : _num(num), _denom(denom) {
    assert(denom != 0);
  }
  void invert() noexcept { std::swap(_num, _denom); }
  ratio operator+(ratio j) const noexcept {
    auto lcm = std::lcm(_denom, j._denom);
    return ratio(_num * (lcm / _denom) + j._num * (lcm / j._denom), lcm);
  }
  bool operator==(ratio j) const noexcept {
    return (_num == j._num) && (_denom == j._denom);
  }
  std::string fomat() const { return fmt::format("{}/{}", _num, _denom); }

private:
  T _num;
  T _denom;
};

// Simply inserts in vector if it is first instance otherwise increments only;
void insert_in_vec(std::vector<int> &v, int a) {
  auto itr = std::ranges::find(v, a);
  if (itr == v.end()) {
    v.push_back(a);
  } else {
    *itr += 1;
  }
}

// Simplifies the sequence <a₁, a₂, a₃, a₄ ...> where any two may be equal
// to a sequence <b₁, b₂, b₃, b₄, ...> and no two b are same such that
// 2^a₁+2^a₂+2^a₃ ... == a^b₁+2^b₂+2^b₃...
auto simplifier(const std::vector<int> &temp_num) {
  std::vector<int> num;
  std::set<int> ps;
  for (auto i : temp_num) {
    int c = 0;
    if (!ps.contains(i))
      c = std::ranges::count(temp_num, i);
    if (c == 1) {
      insert_in_vec(num, i);
      ps.insert(i);
      continue;
    }
    for (auto j = 1; c > 1; ++j) {
      auto temp_base = std::pow(2, j);
      auto p = std::floor(std::log(c) / log(temp_base));
      if (p < 1)
        break;
      insert_in_vec(num, i + p);
      c -= std::pow(2, j);
    }
    ps.insert(i);
  }
  return num;
}

TEST_CASE("57 Square root convergents", "") {
  const int limit = 1000;
  std::unordered_map<int, std::vector<int>> numerator{{0, {2, 0}}};
  std::unordered_map<int, std::vector<int>> denomenator{{0, {1}}};
  // findes the fraction equal value of continued fraction 2+1/2 for a given
  // depth in terms of summationn of power of 2s
  auto fraction = [&](int depth) noexcept {
    auto temp_num = denomenator[depth - 1];
    auto temp_denom = numerator[depth - 1];
    std::ranges::transform(temp_denom, std::back_inserter(temp_num),
                           [](auto i) noexcept { return i + 1; });
    std::ranges::sort(temp_num);
    auto num = simplifier(temp_num);
    numerator.insert({depth, num});
    denomenator.insert({depth, temp_denom});
  };
  // Find all first 1000 fractions
  for (auto i = 1; i < limit + 1; ++i) {
    fraction(i);
    // fmt::print("{} Done\n", i);
  }

  // Adds 1 to the inverse of fractions found in prev step
  for (auto i = 0ul; i < numerator.size(); ++i) {
    auto &temp_num = numerator[i];
    auto &temp_denom = denomenator[i];
    std::vector<int> num;
    std::ranges::copy(temp_num, std::back_inserter(num));
    std::ranges::copy(temp_denom, std::back_inserter(num));
    auto t = simplifier(num);
    temp_denom = temp_num;
    temp_num = num;
    // fmt::print("Done for {}\n", i);
  }

  // counts the number of digits using log, this helps because
  // log can be approximated without calculating actual value
  auto digit_count = [](const std::vector<int> &n) noexcept {
    auto max = *std::ranges::max_element(n);
    double remaining = 0;
    for (auto i : n) {
      double temp = std::pow(static_cast<double>(2), i - max);
      remaining += temp;
    }
    return std::floor(static_cast<double>(max) * std::log10(2) +
                      std::log10(remaining)) +
           1;
  };

  int count = 0;
  for (auto i = 0ul; i < numerator.size(); ++i) {
    if (digit_count(numerator[i]) > digit_count(denomenator[i]))
      ++count;
  }
  REQUIRE(count == 153);

  // This overflows
  // auto add_2 = [](auto j) { return ratio<std::int64_t>(2, 1) + j; };
  // for (auto i = 0; i < limit; ++i) {
  //   auto ret = add_2(ratio<std::int64_t>(1, 2));
  //   for (auto j = 0; j < i; ++j) {
  //     ret.invert();
  //     ret = add_2(ret);
  //   }
  //   ret.invert();
  //   ret = ret + ratio<std::int64_t>(1, 1);
  //   fmt::print("{}\n", ret.fomat());
  // }
}
