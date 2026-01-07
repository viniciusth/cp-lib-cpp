#pragma once

#include "library/bits/stdc++.h"
#include "library/mod.hpp"

/**
 * @brief Standard Data and Action Policies for Segment Tree.
 *
 * ## Algebraic Concepts
 *
 * ### Data Policy (Monoid)
 * Encapsulates the algebraic structure **(S, op, e)** where:
 * - **S**: The set of values stored in the tree nodes.
 * - **op**: An associative binary operation `S x S -> S` (e.g., max, sum).
 * - **e**: The identity element such that `op(x, e) = op(e, x) = x`.
 *
 * ### Action Policy (Monoid Action / Endomorphisms)
 * Encapsulates the structure **(F, composition, id)** acting on S:
 * - **F**: The set of update functions (operators).
 * - **mapping**: The action `F x S -> S` that applies update `f` to data `x`.
 * - **composition**: An associative operation `F x F -> F` that merges updates.
 * - **id**: The identity update such that `mapping(id, x) = x`.
 *
 * This design strictly adheres to the **Lazy Propagation** framework where
 * updates form a monoid that acts on the data monoid.
 *
 * ## Strategies for Designing Monoids
 * 1. Range Sum / Count:
 *    - Node needs to store `value` AND `size`.
 *    - `op(a, b)` sums both value and size.
 *    - Lazy actions (Add/Set/Affine) need `size` to update `value` correctly.
 *
 * 2. DP on Trees (Maximum Subsegment Sum, etc.):
 *    - Store the full DP state in the Node.
 *    - `op(a, b)` merges two DP states (combine left child state + right child
 * state).
 *    - Example: `max_prefix`, `max_suffix`, `max_sub`, `total_sum`.
 *
 * 3. Hashing (Rolling Hash):
 *    - Node stores `{hash_value, length}`.
 *    - `op(a, b)` combines hashes: `(a.hash * base^b.len + b.hash) % M`.
 *
 * 4. Lazy Propagation:
 *    - Design purely functional Actions (`x + f`, `x = f`).
 *    - Use `Lazy<Action, Data>` to compose them.
 *    - Ensure Data Nodes implement necessary operators (`+`, `*`, `=`).
 */

namespace library::segtree::monoids {

// --- Data Policies ---

/**
 * @brief Range Maximum Query.
 * Node: T
 * Op: max(a, b)
 * Identity: numeric_limits<T>::lowest()
 */
template <typename T> struct RangeMax {
  using S = T;
  static S op(S a, S b) { return std::max(a, b); }
  static S e() { return std::numeric_limits<T>::lowest(); }
};

/**
 * @brief Range Minimum Query.
 * Node: T
 * Op: min(a, b)
 * Identity: numeric_limits<T>::max()
 */
template <typename T> struct RangeMin {
  using S = T;
  static S op(S a, S b) { return std::min(a, b); }
  static S e() { return std::numeric_limits<T>::max(); }
};

/**
 * @brief Range Arg Max Query.
 * Returns {value, index} of the maximum element.
 * Tie-breaking: returns smaller index.
 */
template <typename T> struct RangeArgMax {
  struct S {
    T val;
    int idx;
    bool operator<(const S &other) const {
      if (val != other.val)
        return val < other.val;
      return idx > other.idx; // Prefer smaller index
    }
  };
  static S op(S a, S b) { return (a < b) ? b : a; }
  static S e() {
    return {std::numeric_limits<T>::lowest(), std::numeric_limits<int>::max()};
  }
};

/**
 * @brief Range Arg Min Query.
 * Returns {value, index} of the minimum element.
 * Tie-breaking: returns smaller index.
 */
template <typename T> struct RangeArgMin {
  struct S {
    T val;
    int idx;
    bool operator<(const S &other) const {
      if (val != other.val)
        return val < other.val;
      return idx > other.idx;
    }
  };
  static S op(S a, S b) {
    if (a.val < b.val)
      return a;
    if (b.val < a.val)
      return b;
    return (a.idx < b.idx) ? a : b;
  }
  static S e() {
    return {std::numeric_limits<T>::max(), std::numeric_limits<int>::max()};
  }
};

/**
 * @brief Node for Range Sum queries.
 * Stores value and size of the range.
 * Supports +, *, and = operators for lazy actions.
 */
template <typename T> struct SumNode {
  T value;
  int size;

  // Implicit conversion allows initializing with just a value (size defaults to
  // 1)
  SumNode(T v = 0, int s = 1) : value(v), size(s) {}

  // Operator overloading for Action Policies

  // Range Add: new_sum = old_sum + add_val * size
  SumNode operator+(T x) const { return {value + x * size, size}; }

  // Range Affine: new_sum = a * old_sum + b * size
  SumNode operator*(std::pair<T, T> f) const {
    return {value * f.first + f.second * size, size};
  }

  // Range Set: new_sum = distinct_val * size
  // Note: We use operator= for "setting" the value from an action
  void operator=(T x) { value = x * size; }
};

/**
 * @brief Range Sum Query.
 * Node: SumNode<T> (value, size)
 * Op: {a.val + b.val, a.size + b.size}
 */
template <typename T> struct RangeSum {
  using S = SumNode<T>;
  static S op(S a, S b) { return {a.value + b.value, a.size + b.size}; }
  static S e() { return {0, 0}; }
};

/**
 * @brief Node for Polynomial Updates: P(i) = a*i + b
 * Stores value, count (sum 1), and sum of indices (sum i).
 * All updates are relative to ABSOLUTE indices.
 */
template <typename T> struct PolyNode {
  T val;
  T sum1; // count (size)
  T sumi; // sum of absolute indices

  // Default constructor
  PolyNode(T v = 0, T s1 = 0, T si = 0) : val(v), sum1(s1), sumi(si) {}

  // Range Add Polynomial: P(i) = a*i + b
  // new_val = old_val + sum(a*i + b)
  //         = old_val + a*sum(i) + b*sum(1)
  PolyNode operator+(std::pair<T, T> f) const {
    return {val + f.first * sumi + f.second * sum1, sum1, sumi};
  }
};

/**
 * @brief Range Sum with Polynomial Updates support.
 * Note: You MUST initialize this tree deeply with correct {val, 1, index}
 * nodes.
 */
template <typename T> struct RangePolySum {
  using S = PolyNode<T>;
  static S op(S a, S b) {
    return {a.val + b.val, a.sum1 + b.sum1, a.sumi + b.sumi};
  }
  static S e() { return {0, 0, 0}; }
};

// --- Action Policies ---

/**
 * @brief Action: Add F to every element in range.
 * Requires: S + F -> S
 */
template <typename T> struct RangeAdd {
  using F = T;
  template <typename S> static S mapping(F f, S x) { return x + f; }
  static F composition(F new_op, F old_op) { return new_op + old_op; }
  static F id() { return T(0); }
};

/**
 * @brief Action: Affine Transform (multipy by a, add b).
 * Requires: S * pair<T,T> -> S
 */
template <typename T> struct RangeAffine {
  using F = std::pair<T, T>; // {a, b} -> ax + b
  template <typename S> static S mapping(F f, S x) { return x * f; }
  static F composition(F new_op, F old_op) {
    // new(old(x)) = new.a * (old.a * x + old.b) + new.b
    return {new_op.first * old_op.first,
            new_op.first * old_op.second + new_op.second};
  }
  static F id() { return {1, 0}; }
};

/**
 * @brief Action: Set every element in range to F.
 * Uses a wrapper struct to handle the "identity" state (no update).
 * Requires: S = T (assignment operator)
 */
template <typename T> struct SetAction {
  T val;
  bool has_val;

  // Implicit conversion from T
  SetAction(T v) : val(v), has_val(true) {}
  SetAction() : val(0), has_val(false) {}

  bool operator==(const SetAction &other) const {
    return has_val == other.has_val && (!has_val || val == other.val);
  }
};

template <typename T> struct RangeSet {
  using F = SetAction<T>;

  template <typename S> static S mapping(F f, S x) {
    if (f.has_val) {
      // We use assignment! The Data Node must handle semantics.
      // e.g. SumNode::operator= sets value = val * size.
      x = f.val;
    }
    return x;
  }

  static F composition(F new_op, F old_op) {
    // If new op is a set, it overrides everything.
    return new_op.has_val ? new_op : old_op;
  }

  static F id() { return SetAction<T>(); }
};

/**
 * @brief Action: Add Polynomial P(i) = a*i + b.
 * Requires: S + pair<T,T> -> S
 */
template <typename T> struct RangePolyAdd {
  using F = std::pair<T, T>; // {a, b} -> a*i + b

  template <typename S> static S mapping(F f, S x) { return x + f; }

  static F composition(F new_op, F old_op) {
    // (a1*i + b1) + (a0*i + b0) = (a1+a0)*i + (b1+b0)
    return {new_op.first + old_op.first, new_op.second + old_op.second};
  }

  static F id() { return {0, 0}; }
};

// --- Composition Helper ---

/**
 * @brief Lazy Strategy Composer.
 * fuses a Data Policy and an Action Policy into a Config for SegTree.
 */
template <template <typename> class ActionPolicy,
          template <typename> class DataPolicy, typename T>
struct Lazy {
  using DataConfig = DataPolicy<T>;
  using ActionConfig = ActionPolicy<T>;

  using S = typename DataConfig::S;
  using F = typename ActionConfig::F;

  static S op(S a, S b) { return DataConfig::op(a, b); }
  static S e() { return DataConfig::e(); }

  static S mapping(F f, S x) { return ActionConfig::mapping(f, x); }
  static F composition(F f, F g) { return ActionConfig::composition(f, g); }
  static F id() { return ActionConfig::id(); }
};

} // namespace library::segtree::monoids
