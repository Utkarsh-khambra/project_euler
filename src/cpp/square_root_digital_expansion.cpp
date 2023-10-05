#include <algorithm>
#include <array>
#include <cassert>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <numeric>
#include <ranges>

// Uses the sqrt finding algorithm given in
// https://en.m.wikipedia.org/wiki/Methods_of_computing_square_roots#Decimal_(base_10)

constexpr size_t s = 101;
constexpr size_t s_c = 102;

auto sqrt_by_digit(int n);

template <size_t S> class bignum {
public:
  bignum() noexcept { std::ranges::fill(_nums, 0); }
  bignum(std::initializer_list<int> list) noexcept {
    assert(list.size() < S);
    std::ranges::fill(_nums, 0);
    std::ranges::reverse_copy(list, _nums.rbegin());
  }
  template <size_t SS> bignum(bignum<SS> &&) = delete;
  template <size_t SS> bignum(const bignum<SS> &other) noexcept {
    static_assert(S > SS, "Can't copy bigger to small");
    std::ranges::fill_n(_nums.begin(), S - SS, 0);
    std::ranges::reverse_copy(other, _nums.rbegin());
  }
  void shift_left(int n) {
    assert(n > 0);
    std::shift_left(_nums.begin(), _nums.end(), n);
    std::fill(_nums.begin() + (S - static_cast<size_t>(n)), _nums.end(), 0);
  }
  template <size_t SS> auto operator+(const bignum<SS> &other) noexcept {
    static_assert(S >= SS, "Adding bigger to small will overflow");
    int carry = 0;
    int j = SS - 1;
    for (auto i = _nums.rbegin(); i != _nums.rend() && j >= 0; ++i, --j) {
      auto temp = *i + other.begin()[j] + carry;
      *i = temp % 10;
      carry = temp / 10;
    }
    return carry;
  }
  auto operator+(int x) noexcept {
    for (auto i = _nums.rbegin(); i != _nums.rend(); ++i) {
      auto temp = *i + x;
      *i = temp % 10;
      x = temp / 10;
    }
    assert(x == 0);
  }
  void insert(int n) noexcept {
    auto i = S - 1;
    while (n != 0) {
      _nums[i] = n % 10;
      n = n / 10;
      --i;
    }
  }
  template <size_t SS> auto operator-(const bignum<SS> &other) noexcept {
    static_assert(S >= SS, "Subtract big from small");
    // print();
    // other.print();
    std::ranges::transform(_nums, _nums.begin(),
                           [](auto i) noexcept { return 9 - i; });
    auto c = *this + other;
    assert(c == 0);
    std::ranges::transform(_nums, _nums.begin(),
                           [](auto i) noexcept { return 9 - i; });
    return c;
  }
  void operator*(int x) noexcept {
    assert(x < 21);
    int c = 0;
    // fmt::print("{}\n", x);
    // print();
    for (auto &i : _nums | std::views::reverse) {
      auto temp = i * x + c;
      i = temp % 10;
      c = temp / 10;
    }
    assert(c == 0);
  }
  bool operator==(const bignum &other) const noexcept {
    return std::ranges::equal(_nums, other._nums);
  }
  auto begin() noexcept { return _nums.begin(); }
  auto end() noexcept { return _nums.end(); }
  auto begin() const noexcept { return _nums.cbegin(); }
  auto end() const noexcept { return _nums.cend(); }
  void print() const noexcept {
    for (auto i :
         _nums | std::views::drop_while([](auto i) noexcept { return i == 0; }))
      fmt::print("{}", i);
    fmt::print("\n");
  }
  template <size_t SS> bool operator>(const bignum<SS> &other) {

    if constexpr (S >= SS) {
      size_t diff = S - SS;
      if (std::any_of(_nums.begin(), _nums.begin() + diff,
                      [](auto i) { return i != 0; }))
        return true;
      for (size_t i = 0; i < SS; ++i, ++diff) {

        if (_nums[diff] > other.begin()[i])
          return true;
        if (_nums[diff] < other.begin()[i])
          return false;
      }
    }
    if constexpr (S < SS) {
      size_t diff = SS - S;
      if (std::any_of(other.begin(), other.begin() + diff,
                      [](auto i) { return i != 0; }))
        return false;
      for (size_t i = 0; i < SS; ++i, ++diff) {

        if (_nums[i] > other.begin()[diff])
          return true;
        if (_nums[i] < other.begin()[diff])
          return false;
      }
    }

    return false;
  }

private:
  std::array<int, S> _nums;
};

auto sqrt_by_digit(int n) {
  int c_digit = std::floor(std::log10(n)) + 1;
  c_digit += (c_digit & 1) - 2;
  bignum<s> sqrt;
  bignum<s_c> c;
  c.insert(n / std::pow(10, c_digit));
  n = n % static_cast<int>(std::pow(10, c_digit));
  size_t current_prec = 0;
  const bignum<s_c> zero;
  auto condition = [](bignum<s_c> t, auto x, const bignum<s_c> &ct) noexcept {
    t.insert(x);
    t *x;
    return t > ct;
  };
  while (current_prec < s - 1) {
    int x = 9;
    bignum<s_c> temp(sqrt);
    temp * 2;
    temp.shift_left(1);
    while (condition(temp, x, c) && x >= 0)
      --x;
    temp + x;
    temp *x;
    c - temp;
    sqrt.shift_left(1);
    sqrt.insert(x);
    if (c == zero) {
      if (c_digit == 0) {
        break;
      }
    }
    c.shift_left(2);
    if (c_digit > 0) {
      c_digit -= 2;
      c.insert(n / std::pow(10, c_digit));
    }
    ++current_prec;
  }
  return std::accumulate(sqrt.begin(), sqrt.end(), 0ul);
}

TEST_CASE("80 Square root digital expansion", "") {
  std::uint64_t sum = 0;
  auto numbers =
      std::views::iota(2, 101) | std::views::filter([](auto a) noexcept {
        int sqrt = std::sqrt(a);
        return !(sqrt * sqrt == a);
      });
  for (auto i : numbers) {
    // fmt::print("Done for {}\n", i);
    sum += sqrt_by_digit(i);
  }
  REQUIRE(sum == 40886);
}
