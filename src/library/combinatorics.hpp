#pragma once

#include "library/bits/stdc++.h"
#include "library/mod.hpp"

namespace lib {

enum CombinatoricsFeature : uint32_t { Factorial = 1 << 0, All = Factorial };

template <const uint32_t M> class Combinatorics {
public:
  using Mint = mod::ModularInt<M>;
  CombinatoricsFeature features;
  std::vector<Mint> f, fi;

  Combinatorics(int n, CombinatoricsFeature features = All)
      : features(features) {
    if (features & Factorial) {
      f.assign(n, 1);
      fi.assign(n, 1);
      for (int i : std::ranges::views::iota(1, n)) {
        f[i] = f[i - 1] * i;
      }

      fi[n - 1] = f[n - 1].inv();

      for (int i : std::ranges::views::iota(0, n - 1) | std::views::reverse) {
        fi[i] = fi[i + 1] * (i + 1);
      }
    }
  }

  Mint C(int n, int k) const {
    if (!(features & Factorial)) {
      throw std::runtime_error(
          "Factorial feature not enabled for Combinatorics");
    }
    if (k < 0 || k > n)
      return 0;
    return f[n] * fi[k] * fi[n - k];
  }

  Mint P(int n, int k) const {
    if (!(features & Factorial)) {
      throw std::runtime_error(
          "Factorial feature not enabled for Combinatorics");
    }
    if (k < 0 || k > n)
      return 0;
    return f[n] * fi[n - k];
  }
};

template <const uint32_t M>
Combinatorics<M> combinatorics(int n, CombinatoricsFeature features = All) {
  return Combinatorics<M>(n, features);
}

} // namespace lib
