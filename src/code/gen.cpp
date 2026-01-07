#include "library/bits/stdc++.h"

using namespace std;

int main() {
  mt19937 rng(random_device{}());
  println("1");

  int n = uniform_int_distribution<int>(1, 4)(rng);
  println("{}", n);
  for (int j = 0; j < 3; j++)
    for (int i = 0; i < n; i++) {
      if (i > 0)
        print(" ");
      int k = uniform_int_distribution<int>(1, 3 * n)(rng);
      print("{}", k);
      if (i == n - 1)
        println();
    }

  return 0;
}
