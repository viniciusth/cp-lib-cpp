#include "library/bits/stdc++.h"

#include "library/library.hpp"

using ll = long long;

//--Range ADD + Global Maximum.
const int maxn = 200100;
const int INF = -maxn;
template <class T> class SegmentTree {
public:
  T t[4 * maxn], up[4 * maxn];
  T merge(T a, T b) { return std::max(a, b); }
  void prop(int cur, int L, int R) {
    if (up[cur] == 0)
      return;
    t[cur] += up[cur];
    if (L != R) {
      up[2 * cur] += up[cur];
      up[2 * cur + 1] += up[cur];
    }
    up[cur] = 0;
  }
  void update(int l, int r, T x, int cur = 1, int L = 0, int R = maxn - 1) {
    prop(cur, L, R);
    if (R < l or L > r)
      return;
    if (L >= l && R <= r) {
      up[cur] += x;
      prop(cur, L, R);
      return;
    }
    int mid = L + R >> 1;
    update(l, r, x, cur * 2, L, mid);
    update(l, r, x, cur * 2 + 1, mid + 1, R);
    t[cur] = merge(t[cur * 2], t[cur * 2 + 1]);
  }
  // Returns {max_value, index_of_max_value}
  std::pair<int, int> query(int l, int r, int cur = 1, int L = 0,
                            int R = maxn - 1) {
    prop(cur, L, R);

    if (L > r || R < l)
      return {INF, -1};

    if (L == R)
      return {t[cur], L};

    if (L >= l && R <= r) {
      int mid = (L + R) >> 1;
      prop(cur * 2, L, mid);
      prop(cur * 2 + 1, mid + 1, R);

      if (t[cur * 2] >= t[cur * 2 + 1]) {
        return query(l, r, cur * 2, L, mid);
      } else {
        return query(l, r, cur * 2 + 1, mid + 1, R);
      }
    }

    int mid = (L + R) >> 1;
    std::pair<int, int> p1 = query(l, r, cur * 2, L, mid);
    std::pair<int, int> p2 = query(l, r, cur * 2 + 1, mid + 1, R);

    if (p1.first >= p2.first)
      return p1;
    return p2;
  }

  int get_max_index(int l, int r) {
    auto [f, s] = query(l, r);
    // std::println("QUERY = {} {}", f, s);
    return s;
  }
};
SegmentTree<int> seg;

void solve() {
  int n;
  std::cin >> n;
  std::set<int> adj[n];
  for (int i = 0; i < n - 1; i++) {
    int a, b;
    std::cin >> a >> b;
    a--;
    b--;
    adj[a].insert(b);
    adj[b].insert(a);
  }

  std::vector<int> positions, positions_idx, where(n), q, cnt_d, vis(n);
  std::vector<std::pair<int, int>> children_range(n, {-1, -1}), dist_range;
  int cur = 0;
  q.push_back(0);
  positions.push_back(adj[0].size());
  positions_idx.push_back(0);
  where[0] = 0;
  seg.update(cur, cur, adj[0].size());
  // std::println("{} = {}", cur, adj[0].size());
  cur++;

  std::set<int> available_dist;
  int cur_dist_range = 0;
  while (!q.empty()) {
    std::vector<int> nq;
    available_dist.insert(dist_range.size());
    // [l, r)
    dist_range.push_back({cur_dist_range, cur_dist_range + q.size()});
    cnt_d.push_back(q.size());
    cur_dist_range += q.size();
    for (int u : q) {
      int st = positions.size();
      for (int v : adj[u]) {
        if (children_range[v].first != -1) {
          continue;
        }
        positions.push_back(adj[v].size());
        seg.update(cur, cur, adj[v].size());
        // std::println("{} = {}", cur, adj[v].size());
        positions_idx.push_back(v);
        where[v] = cur;
        cur++;
        nq.push_back(v);
      }
      // [l, r)
      children_range[u] = {st, cur};
    }
    std::swap(q, nq);
  }

  // std::println("{}", where[12]);

  int ans = 0;
  int selected = 0;
  while (selected < n) {
    ans++;
    auto it = available_dist.begin();
    std::vector<int> group;
    int last_selected = -1;
    while (it != available_dist.end()) {
      int d = *it;

      // std::println("cur dist = {}", d);

      auto [ld, rd] = dist_range[d];
      rd--;
      // grab max from segtree
      int u = -1;
      auto [lf, rf] = last_selected == -1 ? std::pair<int, int>{-1, -1}
                                          : children_range[last_selected];
      rf--;
      if (rf < lf)
        rf = lf = -1;
      // std::println("dist range: [{}, {}], forbidden: [{}, {}]", ld, rd, lf,
      // rf);

      if (lf == -1) {
        u = seg.get_max_index(ld, rd);
      } else {
        if (ld < lf) {
          u = seg.get_max_index(ld, lf - 1);
          // std::println("trying left {}", u);
        }
        if (rf < rd) {
          int v = seg.get_max_index(rf + 1, rd);
          // std::println("trying right {}", v);
          if (u == -1 || positions[v] > positions[u])
            u = v;
        }
      }
      if (u == -1 || vis[u]) {
        // std::println("skipped");
        last_selected = -1;
        it++;
        continue;
      }
      cnt_d[d]--;
      if (cnt_d[d] == 0)
        available_dist.erase(it);

      // std::println("Point selected: {} ({})", u, positions_idx[u]);
      vis[u] = 1;
      seg.update(u, u, -n);
      positions[u] -= n;
      u = positions_idx[u];
      group.push_back(u + 1);
      last_selected = u;

      // readjust adjacency
      for (int v : adj[u]) {
        seg.update(where[v], where[v], -1);
        adj[v].erase(u);
        positions[v]--;
      }

      it = available_dist.upper_bound(d);
    }
    // library::io::print_vec(group);
    selected += group.size();
  }

  std::println("{}", ans);
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
