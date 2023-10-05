#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <numeric>
#include <unordered_map>
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

// finds the highest power of b in a!
// See Legender's/de polignac's formula
static int factorize_factorial(uint64_t a, uint64_t b) {
  int highest_power = 0;
  for (int i = 1; true; ++i) {
    uint64_t temp = a / std::pow(b, i);
    if (temp == 0)
      return highest_power;
    highest_power += temp;
  }
}
int main() {
  auto primes = sieve_of_eratosthens(100);
  std::unordered_map<int, int> factoriazation;
  for (auto i : primes)
    factoriazation[static_cast<size_t>(i)] = 0;
  for (auto &i : factoriazation)
    i.second = factorize_factorial(100, i.first);
  // Remove least significant zeros
  factoriazation[2] -= factoriazation[5];
  factoriazation.erase(5);

  int num_of_digits = std::floor(std::accumulate(
                          factoriazation.begin(), factoriazation.end(), 0.0,
                          [](double accu, const auto &a) {
                            return accu + a.second * std::log10(a.first);
                          })) +
                      1;
  fmt::print("{}\n", num_of_digits);
}
