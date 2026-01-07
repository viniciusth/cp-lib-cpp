#include "library/bits/stdc++.h"
#include "library/library.hpp"

using namespace library::segtree;
using namespace library::mod;

std::mt19937 rng(42);

int rand_int(int l, int r) {
  return std::uniform_int_distribution<int>(l, r)(rng);
}

// --- Test 1: RMQ (Point Update, Range Max) ---
void test_rmq() {
  std::cout << "Testing RMQ..." << std::endl;
  int n = 100;
  RangeMaxTree<long long> seg(n);
  std::vector<long long> naive(n, std::numeric_limits<long long>::lowest());

  for (int i = 0; i < 1000; i++) {
    int type = rand_int(0, 1);
    if (type == 0) { // Update
      int p = rand_int(0, n - 1);
      long long x = rand_int(-1000, 1000);
      seg.set(p, x);
      naive[p] = x;
    } else { // Query
      int l = rand_int(0, n);
      int r = rand_int(l, n);
      long long ans = seg.prod(l, r);
      long long true_ans = std::numeric_limits<long long>::lowest();
      for (int k = l; k < r; k++)
        true_ans = std::max(true_ans, naive[k]);
      assert(ans == true_ans);
    }
  }
  std::cout << "RMQ Passed!" << std::endl;
}

// --- Test 2: Range Add Range Sum (Lazy) ---
void test_lazy_sum() {
  std::cout << "Testing Lazy Sum..." << std::endl;
  using Mint = ModularInt<998244353>;
  int n = 50;

  // Initialize with size=1 nodes
  using S = monoids::RangeSum<Mint>::S;
  std::vector<S> init(n, S(0, 1));
  LazyRangeAddSumTree<Mint> seg(init);

  std::vector<Mint> naive(n, 0);

  for (int i = 0; i < 1000; i++) {
    int type = rand_int(0, 1);
    int l = rand_int(0, n);
    int r = rand_int(l, n);
    if (type == 0) { // Range Add
      int x_val = rand_int(0, 100);
      Mint x = x_val;
      seg.apply(l, r, x);
      for (int k = l; k < r; k++)
        naive[k] += x;
    } else { // Range Sum
      Mint ans = seg.prod(l, r).value;
      Mint true_ans = 0;
      for (int k = l; k < r; k++)
        true_ans += naive[k];
      assert(ans == true_ans);
    }
  }
  std::cout << "Lazy Sum Passed!" << std::endl;
}

// --- Test 3: Range Polynomial Update Range Sum (Advanced) ---
// Using library components: Lazy<RangePolyAdd, RangePolySum>
struct PolyConfig : monoids::Lazy<monoids::RangePolyAdd, monoids::RangePolySum,
                                  ModularInt<1000000007>> {};

void test_poly_update() {
  std::cout << "Testing Range Polynomial Update..." << std::endl;
  using Mint = ModularInt<1000000007>;

  int n = 50;

  using S = monoids::PolyNode<Mint>;
  std::vector<S> init(n);
  for (int i = 0; i < n; ++i)
    init[i] = {0, 1, i}; // val=0, count=1, sum_idx=i

  LazySegTree<PolyConfig> seg(init);
  std::vector<Mint> naive(n, 0);

  for (int i = 0; i < 2000; i++) {
    int type = rand_int(0, 1);
    int l = rand_int(0, n);
    int r = rand_int(l, n);

    if (type == 0) { // Add P(i) = a*i + b
      int a_val = rand_int(0, 10);
      int b_val = rand_int(0, 10);
      PolyConfig::F f = {a_val, b_val};
      seg.apply(l, r, f);
      for (int k = l; k < r; k++) {
        naive[k] += f.first * k + f.second;
      }
    } else { // Range Sum
      Mint ans = seg.prod(l, r).val;
      Mint true_ans = 0;
      for (int k = l; k < r; k++)
        true_ans += naive[k];
      assert(ans == true_ans);
    }
  }
  std::cout << "Polynomial Update Passed!" << std::endl;
}

// --- Test 4: Range Set Range Sum ---
// Using library components: Lazy<RangeSet, RangeSum>
struct SetSumConfig : monoids::Lazy<monoids::RangeSet, monoids::RangeSum, int> {
};

void test_range_set() {
  std::cout << "Testing Range Set..." << std::endl;
  int n = 50;
  using S = monoids::SumNode<int>;
  std::vector<S> init(n, S(0, 1));
  LazySegTree<SetSumConfig> seg(init);
  std::vector<int> naive(n, 0);

  for (int i = 0; i < 1000; i++) {
    int type = rand_int(0, 2); // 0=Set, 1=Sum, 2=Add(if supported? No just Set)
    int l = rand_int(0, n);
    int r = rand_int(l, n);

    if (type == 0) { // Range Set
      int val = rand_int(-100, 100);
      seg.apply(l, r, val); // Implicit conversion to SetAction
      for (int k = l; k < r; ++k)
        naive[k] = val;
    } else { // Range Sum
      int ans = seg.prod(l, r).value;
      int true_ans = 0;
      for (int k = l; k < r; ++k)
        true_ans += naive[k];
      assert(ans == true_ans);
    }
  }
  std::cout << "Range Set Passed!" << std::endl;
}

// --- Test 5: Range Arg Min/Max ---
void test_arg_min_max() {
  std::cout << "Testing Range Arg Min/Max..." << std::endl;
  int n = 50;

  // ArgMax Test
  SegTree<monoids::RangeArgMax<int>> seg_max(n);
  std::vector<int> naive(n, std::numeric_limits<int>::lowest());

  for (int i = 0; i < n; ++i) {
    seg_max.set(i, {naive[i], i});
  }

  for (int i = 0; i < 1000; i++) {
    int type = rand_int(0, 1);
    if (type == 0) { // Update
      int p = rand_int(0, n - 1);
      int x = rand_int(-100, 100);
      seg_max.set(p, {x, p});
      naive[p] = x;
    } else { // Query
      int l = rand_int(0, n);
      int r = rand_int(l, n);
      auto ans = seg_max.prod(l, r);

      int true_val = std::numeric_limits<int>::lowest();
      int true_idx = std::numeric_limits<int>::max();

      for (int k = l; k < r; k++) {
        if (naive[k] > true_val) {
          true_val = naive[k];
          true_idx = k;
        } else if (naive[k] == true_val) {
          if (k < true_idx)
            true_idx = k;
        }
      }

      assert(ans.val == true_val);
      assert(ans.idx == true_idx);
    }
  }

  // ArgMin Test
  SegTree<monoids::RangeArgMin<int>> seg_min(n);
  std::fill(naive.begin(), naive.end(), std::numeric_limits<int>::max());

  for (int i = 0; i < n; ++i) {
    seg_min.set(i, {naive[i], i});
  }

  for (int i = 0; i < 1000; i++) {
    int type = rand_int(0, 1);
    if (type == 0) { // Update
      int p = rand_int(0, n - 1);
      int x = rand_int(-100, 100);
      seg_min.set(p, {x, p});
      naive[p] = x;
    } else { // Query
      int l = rand_int(0, n);
      int r = rand_int(l, n);
      auto ans = seg_min.prod(l, r);

      int true_val = std::numeric_limits<int>::max();
      int true_idx = std::numeric_limits<int>::max();

      for (int k = l; k < r; k++) {
        if (naive[k] < true_val) {
          true_val = naive[k];
          true_idx = k;
        } else if (naive[k] == true_val) {
          if (k < true_idx)
            true_idx = k;
        }
      }

      assert(ans.val == true_val);
      assert(ans.idx == true_idx);
    }
  }

  std::cout << "Arg Min/Max Passed!" << std::endl;
}

int main() {
  test_rmq();
  test_lazy_sum();
  test_poly_update();
  test_range_set();
  test_arg_min_max();
  return 0;
}
