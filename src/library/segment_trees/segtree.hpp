#pragma once

#include "library/bits/stdc++.h"
#include "library/segment_trees/segtree_configs.hpp"

namespace lib::segtree {

template <class Config> class SegTree {
public:
  using S = typename Config::S;

  SegTree() : SegTree(0) {}
  explicit SegTree(int n) : SegTree(std::vector<S>(n, Config::e())) {}
  explicit SegTree(const std::vector<S> &v) : _n(int(v.size())) {
    log = 0;
    while ((1U << log) < (unsigned int)(_n))
      log++;
    size = 1 << log;
    d = std::vector<S>(2 * size, Config::e());
    for (int i = 0; i < _n; i++)
      d[size + i] = v[i];
    for (int i = size - 1; i >= 1; i--)
      update(i);
  }

  /**
   * @brief Sets the value at index p to x.
   * "Point Set" - overwrites the value.
   * Complexity: O(log n)
   */
  void set(int p, S x) {
    assert(0 <= p && p < _n);
    p += size;
    d[p] = x;
    for (int i = 1; i <= log; i++)
      update(p >> i);
  }

  /**
   * @brief Helper to get the value at index p.
   * Complexity: O(1)
   */
  S get(int p) const {
    assert(0 <= p && p < _n);
    return d[p + size];
  }

  /**
   * @brief Returns the product of the range [l, r).
   * "Product" refers to the Monoid binary operation (sum, max, min, etc.).
   * Complexity: O(log n)
   */
  S prod(int l, int r) const {
    assert(0 <= l && l <= r && r <= _n);
    S sml = Config::e(), smr = Config::e();
    l += size;
    r += size;

    while (l < r) {
      if (l & 1)
        sml = Config::op(sml, d[l++]);
      if (r & 1)
        smr = Config::op(d[--r], smr);
      l >>= 1;
      r >>= 1;
    }
    return Config::op(sml, smr);
  }

  /**
   * @brief Returns the product of the entire range [0, n).
   */
  S all_prod() const { return d[1]; }

  /**
   * @brief Binary search on the tree (finds largest index satisfying
   * condition). Condition: pred must be monotonous (T, T, ..., T, F, F, ...).
   */
  template <bool (*f)(S)> int max_right(int l) const {
    return max_right(l, [](S x) { return f(x); });
  }
  template <class F> int max_right(int l, F f) const {
    assert(0 <= l && l <= _n);
    assert(f(Config::e()));
    if (l == _n)
      return _n;
    l += size;
    S sm = Config::e();
    do {
      while (l % 2 == 0)
        l >>= 1;
      if (!f(Config::op(sm, d[l]))) {
        while (l < size) {
          l = (2 * l);
          if (f(Config::op(sm, d[l]))) {
            sm = Config::op(sm, d[l]);
            l++;
          }
        }
        return l - size;
      }
      sm = Config::op(sm, d[l]);
      l++;
    } while ((l & -l) != l);
    return _n;
  }

  /**
   * @brief Binary search on the tree (finds smallest index satisfying
   * condition). Condition: pred must be monotonous (F, F, ..., F, T, T, ...).
   */
  template <bool (*f)(S)> int min_left(int r) const {
    return min_left(r, [](S x) { return f(x); });
  }
  template <class F> int min_left(int r, F f) const {
    assert(0 <= r && r <= _n);
    assert(f(Config::e()));
    if (r == 0)
      return 0;
    r += size;
    S sm = Config::e();
    do {
      r--;
      while (r > 1 && (r % 2))
        r >>= 1;
      if (!f(Config::op(d[r], sm))) {
        while (r < size) {
          r = (2 * r + 1);
          if (f(Config::op(d[r], sm))) {
            sm = Config::op(d[r], sm);
            r--;
          }
        }
        return r + 1 - size;
      }
      sm = Config::op(d[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

private:
  int _n, size, log;
  std::vector<S> d;

  void update(int k) { d[k] = Config::op(d[2 * k], d[2 * k + 1]); }
};

template <class Config> class LazySegTree {
public:
  using S = typename Config::S;
  using F = typename Config::F;

  LazySegTree() : LazySegTree(0) {}
  explicit LazySegTree(int n) : LazySegTree(std::vector<S>(n, Config::e())) {}
  explicit LazySegTree(const std::vector<S> &v) : _n(int(v.size())) {
    log = 0;
    while ((1U << log) < (unsigned int)(_n))
      log++;
    size = 1 << log;
    d = std::vector<S>(2 * size, Config::e());
    lz = std::vector<F>(size, Config::id());
    for (int i = 0; i < _n; i++)
      d[size + i] = v[i];
    for (int i = size - 1; i >= 1; i--)
      update(i);
  }

  /**
   * @brief Sets the value at index p to x.
   * Performs lazy push-down before setting, ensuring consistency.
   * Complexity: O(log n)
   */
  void set(int p, S x) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    d[p] = x;
    for (int i = 1; i <= log; i++)
      update(p >> i);
  }

  /**
   * @brief Gets the value at index p.
   * Performs lazy push-down to retrieve the up-to-date value.
   * Complexity: O(log n)
   */
  S get(int p) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    return d[p];
  }

  /**
   * @brief Returns the product of range [l, r).
   * Complexity: O(log n)
   */
  S prod(int l, int r) {
    assert(0 <= l && l <= r && r <= _n);
    if (l == r)
      return Config::e();

    l += size;
    r += size;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    S sml = Config::e(), smr = Config::e();
    while (l < r) {
      if (l & 1)
        sml = Config::op(sml, d[l++]);
      if (r & 1)
        smr = Config::op(d[--r], smr);
      l >>= 1;
      r >>= 1;
    }

    return Config::op(sml, smr);
  }

  /**
   * @brief Returns the product of range [0, n).
   */
  S all_prod() { return d[1]; }

  /**
   * @brief Applies update f to the element at index p.
   * Complexity: O(log n)
   */
  void apply(int p, F f) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    d[p] = Config::mapping(f, d[p]);
    for (int i = 1; i <= log; i++)
      update(p >> i);
  }

  /**
   * @brief Applies update f to the range [l, r).
   * "Range Update" / "Range Apply".
   * Complexity: O(log n)
   */
  void apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= _n);
    if (l == r)
      return;

    l += size;
    r += size;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    {
      int l2 = l, r2 = r;
      while (l < r) {
        if (l & 1)
          all_apply(l++, f);
        if (r & 1)
          all_apply(--r, f);
        l >>= 1;
        r >>= 1;
      }
      l = l2;
      r = r2;
    }

    for (int i = 1; i <= log; i++) {
      if (((l >> i) << i) != l)
        update(l >> i);
      if (((r >> i) << i) != r)
        update((r - 1) >> i);
    }
  }

  /**
   * @brief Binary search on the tree (finds largest index satisfying
   * condition). Condition: pred must be monotonous (T, T, ..., T, F, F, ...).
   */
  template <bool (*f)(S)> int max_right(int l) {
    return max_right(l, [](S x) { return f(x); });
  }
  template <class G> int max_right(int l, G g) {
    assert(0 <= l && l <= _n);
    assert(g(Config::e()));
    if (l == _n)
      return _n;
    l += size;
    for (int i = log; i >= 1; i--)
      push(l >> i);
    S sm = Config::e();
    do {
      while (l % 2 == 0)
        l >>= 1;
      if (!g(Config::op(sm, d[l]))) {
        while (l < size) {
          push(l);
          l = (2 * l);
          if (g(Config::op(sm, d[l]))) {
            sm = Config::op(sm, d[l]);
            l++;
          }
        }
        return l - size;
      }
      sm = Config::op(sm, d[l]);
      l++;
    } while ((l & -l) != l);
    return _n;
  }

  /**
   * @brief Binary search on the tree (finds smallest index satisfying
   * condition). Condition: pred must be monotonous (F, F, ..., F, T, T, ...).
   */
  template <bool (*f)(S)> int min_left(int r) {
    return min_left(r, [](S x) { return f(x); });
  }
  template <class G> int min_left(int r, G g) {
    assert(0 <= r && r <= _n);
    assert(g(Config::e()));
    if (r == 0)
      return 0;
    r += size;
    for (int i = log; i >= 1; i--)
      push((r - 1) >> i);
    S sm = Config::e();
    do {
      r--;
      while (r > 1 && (r % 2))
        r >>= 1;
      if (!g(Config::op(d[r], sm))) {
        while (r < size) {
          push(r);
          r = (2 * r + 1);
          if (g(Config::op(d[r], sm))) {
            sm = Config::op(d[r], sm);
            r--;
          }
        }
        return r + 1 - size;
      }
      sm = Config::op(d[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

private:
  int _n, size, log;
  std::vector<S> d;
  std::vector<F> lz;

  void update(int k) { d[k] = Config::op(d[2 * k], d[2 * k + 1]); }
  void all_apply(int k, F f) {
    d[k] = Config::mapping(f, d[k]);
    if (k < size)
      lz[k] = Config::composition(f, lz[k]);
  }
  void push(int k) {
    all_apply(2 * k, lz[k]);
    all_apply(2 * k + 1, lz[k]);
    lz[k] = Config::id();
  }
};

// --- Convenience Aliases ---

// Point Update Trees
template <typename T> using RangeMaxTree = SegTree<monoids::RangeMax<T>>;
template <typename T> using RangeMinTree = SegTree<monoids::RangeMin<T>>;
template <typename T> using RangeSumTree = SegTree<monoids::RangeSum<T>>;

// Lazy Update Trees
template <typename T>
using LazyRangeAddSumTree =
    LazySegTree<monoids::Lazy<monoids::RangeAdd, monoids::RangeSum, T>>;

template <typename T>
using LazyRangeAddMaxTree =
    LazySegTree<monoids::Lazy<monoids::RangeAdd, monoids::RangeMax, T>>;

template <typename T>
using LazyRangeAddMinTree =
    LazySegTree<monoids::Lazy<monoids::RangeAdd, monoids::RangeMin, T>>;

template <typename T>
using LazyRangeAffineSumTree =
    LazySegTree<monoids::Lazy<monoids::RangeAffine, monoids::RangeSum, T>>;

} // namespace lib::segtree
