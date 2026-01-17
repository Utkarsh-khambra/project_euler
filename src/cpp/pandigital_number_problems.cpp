#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <sys/types.h>
#include <vector>

constexpr unsigned int pandigital = 0b1111111110;

auto pandigital_score(uint64_t n) {
  unsigned int digits = 1 << (n % 10);

  while ((n = n / 10)) {
    digits |= 1 << n % 10;
  }
  return digits;
}

bool is_pandigital(uint64_t n) { return pandigital_score(n) == pandigital; }

template <typename Itr> bool johnson_trotter(Itr first, Itr last) {
  return false;
}

std::vector<uint64_t> nums;

template <std::random_access_iterator Itr, typename F>
void heap_permute_helper(unsigned int length, Itr first, Itr last, F func) {
  if (length == 1) {
    func(first, last);
  } else {
    heap_permute_helper(length - 1, first, last, func);
    for (auto i = 0; i < length - 1; ++i) {
      if (length % 2 == 0) {
        std::iter_swap(first + i, first + length - 1);
      } else {
        std::iter_swap(first, first + length - 1);
      }
      heap_permute_helper(length - 1, first, last, func);
    }
  }
}

// Heap's algorithm
template <std::random_access_iterator Itr, typename F>
void heap_permute(Itr first, Itr last, F func) {
  heap_permute_helper(std::distance(first, last), first, last, func);
}

void is_pandigital_product() {
  // If the product has less than 4 digits and the multiplicand can't have more
  // than three digits meaning we can't have a 9 digit integer. And if product
  // has more than 4 digits than multiplcant can't have less digits than 4
  // giving us more digites than 9. So product can only be 4 digits that means
  // we can go over all the products of 4 digits and find all the pandigital
  // numbers
  std::set<int> set;
  for (auto i = 1; i < 98; ++i) {
    if (i % 11 == 0) {
      continue;
    }
    for (auto j = 9876 / i; j > i; --j) {
      const auto k = i * j;
      const bool check =
          pandigital ==
          (pandigital_score(k) | pandigital_score(i) | pandigital_score(j));
      if (check)
        set.insert(k);
    }
  }
  std::cout << std::accumulate(set.begin(), set.end(), uint64_t{0});
}

void pandigital_multiple() {
  // Since we want to make a pandigital number of the form
  // N*(1*10^a1+2*10^a2+e*10^a3...), we can enforce some
  // constraints on the possible values of N and how long the
  // sequence of consequtive numbers go.
  // N cannot be a 5 digit number because if it's 5 digit then
  // the resulting number will always have at least 10 digits.
  // So our number can only be <= 4 digits.
  // Now another constraint here is how many 1,2,3 can be used to create
  // the final number if the N is big that means we can't use many number from
  // sequences and by applying some logic we get following loose criteria:
  // If 4 digit then we can only use 1 and 2
  // if 3 digit we can use 1,2,3,4
  // If 2 digit we can use 1,2,3,4,5,6
  // If 1 digit we can use 1,2,3,4,5,6,7,8,9
  // Now we just need to do exhaustive search for the pandigital number
  uint64_t max = 0;
  std::vector<int> s;
  int N;
  for (uint64_t i = 5000; i < 9999; ++i) {
    const auto num = i * 100000 + i * 2;
    if (is_pandigital(num) && max < num) {
      max = num;
    }
  }
  for (uint64_t i = 100; i < 1000; ++i) {
    uint64_t num = i;
    for (auto j = 2; j < 5; ++j) {
      num = num * 1000 * j;
      if (is_pandigital(num) && max < num) {
        max = num;
        break;
      }
    }
  }
  for (uint64_t i = 10; i < 100; ++i) {
    uint64_t num = i;
    for (auto j = 2; j < 7; ++j) {
      num = num * 100 * j;
      if (is_pandigital(num) && max < num) {
        max = num;
        break;
      }
    }
  }
  for (uint64_t i = 2; i < 10; ++i) {
    uint64_t num = i;
    for (auto j = 2; j < 7; ++j) {
      num = num * 10 * j;
      if (is_pandigital(num) && max < num) {
        max = num;
        break;
      }
    }
  }
  std::cout << max;
}

bool is_prime(uint64_t n) {
  for (auto i = 2; i <= n - 1; ++i)
    if (n % i == 0) {
      // std::cout << i << " Diveides " << n << "\n";
      return false;
    }
  return true;
}

void pandigital_prime() {

  auto print = [](auto first, auto last) {
    uint64_t digits = *(first++);
    for (; last != first; first++) {
      digits = std::fma(digits, 10.0, *first);
    }
    return digits;
  };
  for (auto i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) {
    std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};
    nums.clear();
    for (auto j = 0; j < i; ++j)
      numbers.pop_back();
    heap_permute(numbers.begin(), numbers.end(), print);
    std::cout << nums.size() << "\n";
    std::sort(nums.begin(), nums.end(), std::greater<>{});
    for (auto i : nums) {
      std::cout << i << "\n";
      if (is_prime(i)) {
        std::cout << "answer" << i << "\n";
        return;
      }
    }
  }
}

void substring_divisiblity() {
  uint64_t sum = 0;
  auto print = [&](auto first, auto last) mutable {
    auto yo = [](auto first, auto last) {
      uint64_t digits = *(first++);
      for (; last != first; first++) {
        digits = std::fma(digits, 10.0, *first);
      }
      return digits;
    };
    // std::cout << yo(first, last) << "\n";
    auto make_num = [](int a, int b, int c) { return a * 100 + b * 10 + c; };
    auto d2 = make_num(*(first + 1), *(first + 2), *(first + 3));
    auto d3 = make_num(*(first + 2), *(first + 3), *(first + 4));
    auto d4 = make_num(*(first + 3), *(first + 4), *(first + 5));
    auto d5 = make_num(*(first + 4), *(first + 5), *(first + 6));
    auto d6 = make_num(*(first + 5), *(first + 6), *(first + 7));
    auto d7 = make_num(*(first + 6), *(first + 7), *(first + 8));
    auto d8 = make_num(*(first + 7), *(first + 8), *(first + 9));
    const auto y = (d2 % 2) + (d3 % 3) + (d4 % 5) + (d5 % 7) + (d6 % 11) +
                   (d7 % 13) + (d8 % 17);
    if (y == 0) {
      sum += yo(first, last);
    }
  };
  std::vector<int> numbers{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  heap_permute(numbers.begin(), numbers.end(), print);
  std::cout << sum << "\n";
}

int main() { substring_divisiblity(); }