#include "library/bits/stdc++.h"

#include "library/library.hpp"

using ll = long long;

void solve() {
  int n, x, y;
  std::string s;
  std::cin >> n >> x >> y >> s;
  auto p = library::io::read_vec<ll>(n);
  int total = 0;
  int a_extra = 0;
  int b_extra = 0;
  bool has_a = false;
  bool has_b = false;
  for (int i = 0; i < n; i++) {
    int d = p[i] / 2;
    x -= d;
    y -= d;
    if (s[i] == '0')
      x--, a_extra += d, has_a = true;
    else
      y--, b_extra += d, has_b = true;
  }
  if (x < 0) {
    int use = std::min(-x, a_extra);
    x += use;
  }
  if (y < 0) {
    int use = std::min(-y, b_extra);
    y += use;
  }
  if (x < 0 || y < 0) {
    std::println("NO");
  } else if (x > 0 && !(has_a || y >= x)) {
    std::println("NO");
  } else if (y > 0 && !(has_b || x >= y)) {
    std::println("NO");
  } else {
    std::println("YES");
  }
}

int main() {
  library::io::setup_io();

  int t = 1;
  std::cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
