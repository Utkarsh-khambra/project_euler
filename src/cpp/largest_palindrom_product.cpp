#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <numeric>
#include <vector>
// Palindrom generation
void helper(int n, int i, int j, std::vector<int> &v, std::string num) {
  if (j < i) {
    int a;
    std::from_chars(num.data(), num.data() + num.size(), a);
    v.push_back(a);
    return;
  }
  for (size_t k = 0; k < 10; ++k) {
    num[static_cast<size_t>(i)] = static_cast<char>(48 + k);
    num[static_cast<size_t>(j)] = static_cast<char>(48 + k);
    helper(n, i + 1, j - 1, v, num);
  }
}
auto get_palindroms(int n) {
  if (n < 2)
    return std::vector<int>();
  std::vector<int> ret;
  for (auto i = 1; i < 10; ++i) {
    std::string s(static_cast<size_t>(n), 48);
    s[static_cast<size_t>(n - 1)] = 48 + i;
    s[0] = 48 + i;
    helper(n, 1, n - 2, ret, s);
  }
  return ret;
}

// Check divisors
bool check_factor(int n) {
  for (auto i = 999; i > 99; --i) {
    if (n % i == 0) {
      auto new_num = n / i;
      if (new_num > 99 && new_num < 1000) {
        return true;
      }
    }
  }
  return false;
}
TEST_CASE("4 Largest palindrome product", "") {
  std::vector<int> palindroms(900 + 900);
  {
    auto p5 = get_palindroms(5);
    auto p6 = get_palindroms(6);
    auto t = std::copy(p5.begin(), p5.end(), palindroms.begin());
    std::copy(p6.begin(), p6.end(), t);
  }
  for (auto i = palindroms.rbegin(); i != palindroms.rend(); ++i) {
    if (check_factor(*i)) {
      REQUIRE(*i == 906609);
      break;
    }
  }
}
