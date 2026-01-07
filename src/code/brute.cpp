#include "library/bits/stdc++.h"

#include "library/library.hpp"

using namespace std;

void solve() {

  int n;
  cin >> n;
  auto a = library::io::read_vec<int>(n);
  auto b = library::io::read_vec<int>(n);
  auto c = library::io::read_vec<int>(n);
  long long ans = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        bool is_good = true;

        for (int z = 0; z < n; z++) {
          int qi = (i + z) % n;
          int qj = (j + z) % n;
          int qk = (k + z) % n;
          is_good &= a[qi] < b[qj] && b[qj] < c[qk];
        }

        ans += is_good;
      }
    }
  }

  println("{}", ans);
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
