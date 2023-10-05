/*
 * Helpfull links I found while solving this
 * https://math.stackexchange.com/questions/81228/modular-exponentiation-by-hand-ab-bmod-c
 * https://stackoverflow.com/questions/12168348/ways-to-do-modulo-multiplication-with-primitive-types
 * https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-multiply-overflow
 * */
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <numeric>
auto constexpr last_digits = static_cast<std::uint64_t>(10'000'000'000);

// Correctly finds a*b mod m if all a, b and m are uint64_t but a*b is bigger
// than uint64_t
uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t res = 0;
  while (a != 0) {
    if (a & 1)
      res = (res + b) % m;
    a >>= 1;
    b = (b << 1) % m;
  }
  return res;
}
static bool check_overflow_on_mult(std::uint64_t a, std::uint64_t b) {
  return a > std::numeric_limits<std::uint64_t>::max() / b;
}

// Returns value of base^exp mod modulus
std::uint64_t power_mod(std::uint64_t base, std::uint64_t exp,
                        std::uint64_t modulus = last_digits) {
  if (exp == 0 || base == 1)
    return 1;
  if (exp % 2 == 0) {
    exp /= 2;
    if (check_overflow_on_mult(base, base)) {
      base = mulmod(base, base, modulus);
    } else {
      base = (base * base) % modulus;
    }
    return power_mod(base, exp, modulus);
  }
  exp -= 1;
  auto temp = power_mod(base, exp, modulus);
  if (check_overflow_on_mult(base, temp)) {
    return mulmod(temp, base, modulus);
  }
  return (base * temp) % modulus;
}

TEST_CASE("48 Self powers", "") {
  std::uint64_t sum = 0;
  for (auto i = 1; i < 1000; ++i) {
    if (i % 10 == 0)
      continue;
    auto t = power_mod(i, i);
    sum += t;
  }
  REQUIRE((sum % last_digits) == 9110846700);
}

// A easier solution that I missed and overengineered another one
// int main() {
// auto sum = 0;
// for (std::uint64_t x = 1; x <= 1000; x++) {
//   std::uint64_t l = x;
//   for (std::uint64_t y = 0; y < x - 1; y++) {
//     l = (l * x) % last_digits;
//   }
//   sum = (sum + l) % last_digits;
// }
// fmt::print("{}\n", sum);
// }
