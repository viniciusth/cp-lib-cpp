#pragma once

#include "library/bits/stdc++.h"

namespace library::mod {

using ll = long long;
template <const ll M> ll binpow(ll a, ll b) {
  ll ans = 1;
  while (b) {
    if (b & 1)
      ans = ans * a % M;
    a = a * a % M;
    b >>= 1;
  }
  return ans;
}

} // namespace library::mod
