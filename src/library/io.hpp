#pragma once

#include "library/bits/stdc++.h"

namespace library::io {

inline void setup_io() {
#ifdef FILE_IO
  if (!freopen("input.txt", "r", stdin)) {
    std::cerr << "No input.txt found.\n";
  }
  if (!freopen("output.txt", "w", stdout)) {
    std::cerr << "No output.txt found.\n";
  }
#endif
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
}

template <typename T> void print_vec(const std::vector<T> &v) {
  for (size_t i = 0; i < v.size(); ++i) {
    std::cout << v[i] << (i == v.size() - 1 ? "" : " ");
  }
  std::cout << "\n";
}

template <typename T> std::pair<std::vector<T>, int> read_vec() {
  int n;
  std::cin >> n;
  std::vector<T> v(n);
  for (T &x : v) {
    std::cin >> x;
  }
  return {v, n};
}

template <typename T> std::vector<T> read_vec(int n) {
  std::vector<T> v(n);
  for (T &x : v) {
    std::cin >> x;
  }
  return v;
}

} // namespace library::io
