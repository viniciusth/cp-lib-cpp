#include "library/bits/stdc++.h"
#include "library/library.hpp"

using namespace library::segtree;
using namespace library::mod;

std::mt19937 rng(42);

int rand_int(int l, int r) {
  return std::uniform_int_distribution<int>(l, r)(rng);
}

// --- Test 1: Persistence Verification (RMQ) ---
// Verify we can access old versions unmodified.
void test_persistence_rmq() {
  std::cout << "Testing Persistence (RMQ)..." << std::endl;
  int n = 20;
  PersistentSegTree<monoids::RangeMax<int>> pst(n);

  std::vector<std::vector<int>> history;
  std::vector<int> current(n, monoids::RangeMax<int>::e());
  history.push_back(current);

  // Perform updates creating new versions
  for (int i = 0; i < 50; i++) {
    int p = rand_int(0, n - 1);
    int x = rand_int(0, 100);

    // Create new version
    pst.set(i, p, x);

    // Update naive history
    current[p] = x;
    history.push_back(current);
  }

  // Verify ALL versions
  assert(pst.version_count() == (int)history.size());
  for (int v = 0; v < (int)history.size(); v++) {
    for (int i = 0; i < n; i++) {
      // Point query check
      int val = pst.get(v, i);
      assert(val == history[v][i]);
    }

    // Range query check
    for (int k = 0; k < 10; ++k) {
      int l = rand_int(0, n);
      int r = rand_int(l, n);
      int prod = pst.prod(v, l, r);

      int expected = monoids::RangeMax<int>::e();
      for (int j = l; j < r; ++j)
        expected = std::max(expected, history[v][j]);

      assert(prod == expected);
    }
  }

  std::cout << "Persistence RMQ Passed!" << std::endl;
}

// --- Test 2: K-th Smallest Number (Static Array) ---
// Classic application: Build PST over value range.
void test_kth_smallest() {
  std::cout << "Testing K-th Smallest Number..." << std::endl;

  int n = 100;
  std::vector<int> a(n);
  for (int i = 0; i < n; ++i)
    a[i] = rand_int(0, 1000); // Values up to 1000

  // Coordinate Compression
  std::vector<int> vals = a;
  std::sort(vals.begin(), vals.end());
  vals.erase(std::unique(vals.begin(), vals.end()), vals.end());

  auto get_idx = [&](int x) {
    return std::lower_bound(vals.begin(), vals.end(), x) - vals.begin();
  };

  int m = (int)vals.size();

  // Build PST
  // roots[i] = tree containing counts for a[0...i-1]
  PersistentSegTree<monoids::RangeSum<int>> pst(m);

  // Version 0 is empty (counts all 0)
  for (int x : a) {
    int idx = get_idx(x);
    int current_ver = pst.version_count() - 1;
    int current_val = pst.get(current_ver, idx).value;
    pst.set(current_ver, idx, current_val + 1);
    // Note: pst.set automatically pushes a new root
  }

  // Queries
  // Find k-th smallest in range [l, r) of the original array
  // This requires walking the segment tree (binary search on seg tree)
  // Functional-style walking: compare left child sum of (rootR - rootL)

  auto find_kth = [&](int l, int r, int k) {
    int root_l = pst.get_root(l); // version l (contains 0...l-1)
    int root_r = pst.get_root(r); // version r (contains 0...r-1)

    // We need direct node access to walk down
    const auto &nodes = pst.get_nodes();

    int curr_l = root_l;
    int curr_r = root_r;

    int range_l = 0; // value range bounds
    int range_r = m;

    while (range_l + 1 < range_r) {
      // Left child counts
      int count =
          nodes[nodes[curr_r].l].val.value - nodes[nodes[curr_l].l].val.value;
      int mid = (range_l + range_r) / 2;

      if (k < count) {
        curr_l = nodes[curr_l].l;
        curr_r = nodes[curr_r].l;
        range_r = mid;
      } else {
        k -= count;
        curr_l = nodes[curr_l].r;
        curr_r = nodes[curr_r].r;
        range_l = mid;
      }
    }
    return vals[range_l];
  };

  for (int i = 0; i < 50; ++i) {
    int l = rand_int(0, n);
    int r = rand_int(l + 1, n); // non-empty
    int k = rand_int(0, r - l - 1);

    // Naive
    std::vector<int> sub;
    for (int j = l; j < r; ++j)
      sub.push_back(a[j]);
    std::sort(sub.begin(), sub.end());
    int expected = sub[k];

    int got = find_kth(l, r, k);
    assert(got == expected);
  }

  std::cout << "K-th Smallest Passed!" << std::endl;
}

int main() {
  test_persistence_rmq();
  test_kth_smallest();
  return 0;
}
