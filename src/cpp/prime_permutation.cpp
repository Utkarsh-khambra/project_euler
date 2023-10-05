#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <charconv>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <set>
#include <vector>
static auto sieve_of_eratosthens(size_t n) {
  std::vector<bool> a(n, true);
  size_t sqrt_n = std::sqrt(n);
  for (size_t i = 2; i < sqrt_n; ++i) {
    if (a.at(i)) {
      auto j = i * i;
      for (; j < n; j += i) {
        a.at(j) = false;
      }
    }
  }
  std::vector<std::uint32_t> ret;
  for (size_t i = 2; i < n; ++i) {
    if (a[i])
      ret.push_back(i);
  }
  return ret;
}
bool contains_ap(const std::vector<int> &v) {
  for (auto i = v.begin(); i != v.end(); ++i) {
    for (auto j = i + 1; j != v.end(); ++j) {
      for (auto k = j + 1; k != v.end(); ++k) {
        if (*k - *j == *j - *i) {
          return true;
        }
      }
    }
  }
  return false;
}

TEST_CASE("49 Prime permutations", "") {
  std::vector<std::vector<int>> permuted_prime;
  char nums1[10], nums2[10];
  {
    auto primes = sieve_of_eratosthens(10000);
    auto temp = std::lower_bound(primes.begin(), primes.end(), 1000);
    for (auto i = temp; i != primes.end(); ++i) {
      std::vector<int> t;
      for (auto j = i + 1; j != primes.end(); ++j) {
        auto [ptr1, ec1] = std::to_chars(nums1, nums1 + 10, *i);
        auto [ptr2, ec2] = std::to_chars(nums2, nums2 + 10, *j);
        std::sort(nums1, ptr1);
        std::sort(nums2, ptr2);
        if (std::equal(nums1, ptr1, nums2)) {
          t.push_back(*j);
        }
      }
      if (!t.empty()) {
        t.push_back(*i);
        std::sort(t.begin(), t.end());
        permuted_prime.push_back(t);
      }
    }
  }
  int count = 0;
  std::vector<int> ps;
  for (auto &i : permuted_prime) {
    if (i.size() < 3)
      continue;
    if (contains_ap(i)) {
      {
        ++count;
        ps = i;
      }
      if (count == 2)
        break;
    }
  }
  REQUIRE_THAT(ps,
               Catch::Matchers::Contains(std::vector<int>{2969, 6299, 9629}));
}

// int main() {
// Original solution
// std::set<int> prime;
// auto prime = sieve_of_eratosthens(10000);
// int count = 0;
// for (auto i = prime.begin(); i != prime.end(); ++i) {
//   for (auto j = i + 1; j != prime.end(); ++j) {
//     for (auto k = j + 1; k != prime.end(); ++k) {
//       if (*j - *i == *k - *j) {
//         auto a = std::to_string(*i);
//         auto b = std::to_string(*j);
//         auto c = std::to_string(*k);
//         if (std::ranges::is_permutation(a, b) &&
//             std::ranges::is_permutation(b, c)) {
//           fmt::print("{}{}{}\n", *i, *j, *k);
//           ++count;
//           if (count == 2)
//             return 0;
//         }
//       }
//     }
//   }
// }
// }
