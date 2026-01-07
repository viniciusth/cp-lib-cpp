#include "library/io.hpp"
#include "library/library.hpp"

void solve() {

  int n;
  cin >> n;
  auto a = lib::io::read_vec<int>(n);
  auto b = lib::io::read_vec<int>(n);
  auto c = lib::io::read_vec<int>(n);
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
  lib::io::setup_io();

  int t = 1;
  std::cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
