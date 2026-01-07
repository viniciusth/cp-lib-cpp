#include "library/library.hpp"

void solve() {
  i32 n;
  cin >> n;
  set<i32> adj[n];
  for (i32 i = 0; i < n - 1; i++) {
    i32 a, b;
    cin >> a >> b;
    a--;
    b--;
    adj[a].insert(b);
    adj[b].insert(a);
  }

  vector<int> positions, positions_idx, where(n), q, cnt_d, vis(n);
  vector<pair<int, int>> children_range(n, {-1, -1}), dist_range;
  int cur = 0;
  q.push_back(0);
  positions.push_back(adj[0].size());
  positions_idx.push_back(0);
  where[0] = 0;
  seg.update(cur, cur, adj[0].size());
  // std::println("{} = {}", cur, adj[0].size());
  cur++;

  set<int> available_dist;
  int cur_dist_range = 0;
  while (!q.empty()) {
    vector<int> nq;
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
    swap(q, nq);
  }

  // std::println("{}", where[12]);

  int ans = 0;
  int selected = 0;
  while (selected < n) {
    ans++;
    auto it = available_dist.begin();
    vector<int> group;
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
