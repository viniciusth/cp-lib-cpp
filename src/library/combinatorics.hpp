#include "library/bits/stdc++.h"
#include "library/mod.hpp"

namespace library::combinatorics {

using ll = long long;

template <const ll M>
std::pair<std::vector<ll>, std::vector<ll>> factorial(int n) {
  std::vector<ll> f(n), fi(n);
  f[0] = 1;
  for (int i : std::ranges::views::iota(1, n)) {
    f[i] = f[i - 1] * i % M;
  }

  fi[n - 1] = library::mod::binpow<M>(f[n - 1], M - 2);

  for (int i : std::ranges::views::iota(0, n - 1) | std::views::reverse) {
    fi[i] = fi[i + 1] * (i + 1) % M;
  }

  return {f, fi};
}

template <const ll M>
ll C(int n, int k, std::vector<ll> &f, std::vector<ll> &fi) {
  if (k < 0 || k > n)
    return 0;
  return (f[n] * fi[k] % M) * fi[n - k] % M;
}

template <const ll M>
ll P(int n, int k, std::vector<ll> &f, std::vector<ll> &fi) {
  if (k < 0 || k > n)
    return 0;
  return f[n] * fi[n - k] % M;
}

} // namespace library::combinatorics
