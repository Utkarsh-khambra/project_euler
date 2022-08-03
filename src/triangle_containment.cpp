#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <string>
#include <vector>

struct triangle {
  int x1, y1, x2, y2, x3, y3;
};
template <> struct fmt::formatter<triangle> {
  // Presentation format: 'f' - fixed, 'e' - exponential.
  // Parses format specifications of the form ['f' | 'e'].
  constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
    // [ctx.begin(), ctx.end()) is a character range that contains a part of
    // the format string starting from the format specifications to be parsed,
    // e.g. in
    //
    //   fmt::format("{:f} - point of interest", point{1, 2});
    //
    // the range will contain "f} - point of interest". The formatter should
    // parse specifiers until '}' or the end of the range. In this example
    // the formatter should parse the 'f' specifier and return an iterator
    // pointing to '}'.

    // Parse the presentation format and store it in the formatter:
    auto it = ctx.begin(), end = ctx.end();
    if (it != end)
      *it++;

    // Check if reached the end of the range:
    if (it != end && *it != '}')
      throw format_error("invalid format");

    // Return an iterator past the end of the parsed range:
    return it;
  }

  // Formats the point p using the parsed format specification (presentation)
  // stored in this formatter.
  template <typename FormatContext>
  auto format(const triangle &p, FormatContext &ctx) -> decltype(ctx.out()) {
    // ctx.out() is an output iterator to write to.
    return format_to(ctx.out(), "[({}, {}), ({}, {}), ({}, {})]", p.x1, p.y1,
                     p.x2, p.y2, p.x3, p.y3);
  }
};

bool same_direction(float m, float c, int x1, int y1, int x2, int y2) {
  float t1 = y1 - m * x1 - c;
  float t2 = y2 - m * x2 - c;
  if (std::signbit(t1) == std::signbit(t2))
    return true;
  return false;
}

std::pair<float, float> line(int x1, int y1, int x2, int y2) {
  float m = (y2 - y1) / static_cast<float>(x2 - x1);
  auto c = y1 - m * x1;
  return {m, c};
}

bool inside_trianlge(triangle t, int p, int q) {
  auto [m1, c1] = line(t.x1, t.y1, t.x2, t.y2);
  auto [m2, c2] = line(t.x2, t.y2, t.x3, t.y3);
  auto [m3, c3] = line(t.x1, t.y1, t.x3, t.y3);
  return same_direction(m1, c1, t.x3, t.y3, p, q) &&
         same_direction(m2, c2, t.x1, t.y1, p, q) &&
         same_direction(m3, c3, t.x2, t.y2, p, q);
}

TEST_CASE("102 Triangle containment", "") {
  std::ifstream f("../src/triangle.txt");
  std::vector<triangle> ts;
  std::string l;
  while (std::getline(f, l)) {
    triangle temp;
    auto it = l.find(',');
    auto ptr = l.data();
    std::from_chars(ptr, l.data() + it, temp.x1);
    ptr = l.data() + it + 1;
    it = l.find(',', it + 1);
    std::from_chars(ptr, l.data() + it, temp.y1);
    ptr = l.data() + it + 1;
    it = l.find(',', it + 1);
    std::from_chars(ptr, l.data() + it, temp.x2);
    ptr = l.data() + it + 1;
    it = l.find(',', it + 1);
    std::from_chars(ptr, l.data() + it, temp.y2);
    ptr = l.data() + it + 1;
    it = l.find(',', it + 1);
    std::from_chars(ptr, l.data() + it, temp.x3);
    ptr = l.data() + it + 1;
    std::from_chars(ptr, l.data() + l.size(), temp.y3);
    ts.push_back(temp);
  }
  int sum = 0;
  for (auto i : ts) {
    sum += inside_trianlge(i, 0, 0);
  }
  REQUIRE(sum == 228);
}
