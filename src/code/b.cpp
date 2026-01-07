#include "library/io.hpp"
#include "library/library.hpp"

void solve() {
  int n, x, y;
  string s;
  cin >> n >> x >> y >> s;
  auto p = lib::io::read_vec<i64>(n);
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
    println("NO");
  } else if (x > 0 && !(has_a || y >= x)) {
    println("NO");
  } else if (y > 0 && !(has_b || x >= y)) {
    println("NO");
  } else {
    println("YES");
  }
}

int main() {
  lib::io::setup_io();

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
