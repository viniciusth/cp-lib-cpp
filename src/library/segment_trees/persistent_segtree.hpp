#pragma once

#include "library/bits/stdc++.h"
#include "library/segment_trees/segtree_configs.hpp"

namespace lib::segtree {

/**
 * @brief Persistent Segment Tree.
 *
 * Uses path copying to maintain previous versions of the tree.
 * Memory is managed internally using a std::vector pool.
 *
 * Concepts:
 * - Version: Represented by the index of the root node of that version.
 * - Nodes: Stored in a single vector. Access is via integer indices.
 */
template <class Config> class PersistentSegTree {
public:
  using S = typename Config::S;

  struct Node {
    S val;
    int l, r;
  };

  PersistentSegTree(int n) : _n(n) {
    // Initial empty tree (all identities)
    roots.push_back(build(0, _n));
  }

  PersistentSegTree(const std::vector<S> &v) : _n(int(v.size())) {
    roots.push_back(build(v, 0, _n));
  }

  /**
   * @brief Returns the root index of the initial version (version 0).
   */
  int get_root() const { return roots.empty() ? -1 : roots[0]; }

  /**
   * @brief Returns the root index of specific version.
   */
  int get_root(int version) const {
    if (version < 0 || version >= (int)roots.size())
      return -1;
    return roots[version];
  }

  /**
   * @brief Returns the current number of versions stored.
   */
  int version_count() const { return (int)roots.size(); }

  /**
   * @brief Creates a new version by setting value at p to x.
   * Based on an existing version specified by root_idx.
   * Returns the new root index (which is NOT automatically added to roots list
   * if you use the internal update, but here we might want a clean API).
   *
   * Let's provide an API that creates a NEW version from a previous one.
   *
   * @param version_idx The version index (0-based) to base the update on.
   * @param p Index to update.
   * @param x New value.
   * @return The index of the NEW version (added to roots).
   */
  int set(int version_idx, int p, S x) {
    assert(version_idx >= 0 && version_idx < (int)roots.size());
    int new_root = update_node(roots[version_idx], 0, _n, p, x);
    roots.push_back(new_root);
    return (int)roots.size() - 1;
  }

  /**
   * @brief Manual set that returns the raw node index of the new root.
   * Use this if you want to manage roots yourself (e.g. for a tree of
   * versions).
   */
  int set_manual(int root_node, int p, S x) {
    return update_node(root_node, 0, _n, p, x);
  }

  /**
   * @brief Product query on a specific version.
   * @param version_idx The version index (0-based) to query.
   */
  S prod(int version_idx, int l, int r) const {
    assert(version_idx >= 0 && version_idx < (int)roots.size());
    return query(roots[version_idx], 0, _n, l, r);
  }

  /**
   * @brief Product query using a raw root node index.
   */
  S prod_manual(int root_node, int l, int r) const {
    return query(root_node, 0, _n, l, r);
  }

  /**
   * @brief Get value at p from a specific version.
   */
  S get(int version_idx, int p) const { return prod(version_idx, p, p + 1); }

  /**
   * @brief Get raw node vector (for debugging or advanced usage)
   */
  const std::vector<Node> &get_nodes() const { return nodes; }

private:
  int _n;
  std::vector<Node> nodes;
  std::vector<int> roots; // roots[v] = index in 'nodes' of root of version v

  int new_node(S val, int l, int r) {
    nodes.push_back({val, l, r});
    return (int)nodes.size() - 1;
  }

  int copy_node(int k) {
    if (k == -1)
      return -1; // Should allow null? Prefer valid nodes.
    return new_node(nodes[k].val, nodes[k].l, nodes[k].r);
  }

  int build(int l, int r) {
    if (l + 1 == r) {
      return new_node(Config::e(), -1, -1);
    }
    int m = (l + r) / 2;
    int left_child = build(l, m);
    int right_child = build(m, r);
    return new_node(Config::op(nodes[left_child].val, nodes[right_child].val),
                    left_child, right_child);
  }

  int build(const std::vector<S> &v, int l, int r) {
    if (l + 1 == r) {
      return new_node(v[l], -1, -1);
    }
    int m = (l + r) / 2;
    int left_child = build(v, l, m);
    int right_child = build(v, m, r);
    return new_node(Config::op(nodes[left_child].val, nodes[right_child].val),
                    left_child, right_child);
  }

  int update_node(int k, int l, int r, int p, S x) {
    // Determine range for current node k is [l, r)
    if (l + 1 == r) {
      // Leaf
      return new_node(x, -1, -1);
    }

    int m = (l + r) / 2;
    int new_k = copy_node(k);

    if (p < m) {
      nodes[new_k].l = update_node(nodes[k].l, l, m, p, x);
    } else {
      nodes[new_k].r = update_node(nodes[k].r, m, r, p, x);
    }

    // Update current value
    nodes[new_k].val =
        Config::op(nodes[nodes[new_k].l].val, nodes[nodes[new_k].r].val);
    return new_k;
  }

  S query(int k, int bounds_l, int bounds_r, int q_l, int q_r) const {
    if (q_l >= bounds_r || q_r <= bounds_l) {
      return Config::e();
    }
    if (q_l <= bounds_l && bounds_r <= q_r) {
      return nodes[k].val;
    }
    int m = (bounds_l + bounds_r) / 2;
    return Config::op(query(nodes[k].l, bounds_l, m, q_l, q_r),
                      query(nodes[k].r, m, bounds_r, q_l, q_r));
  }
};

} // namespace lib::segtree
