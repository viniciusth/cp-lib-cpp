#pragma once

#include "library/bits/stdc++.h"

namespace lib::mod {

const uint32_t P1 = 1e9 + 7;
const uint32_t P2 = 998244353;

template <const uint32_t M> struct ModularInt {
  uint32_t v;

  ModularInt() : v(0) {}
  ModularInt(long long _v) {
    long long x = _v % (long long)M;
    if (x < 0)
      x += M;
    v = (uint32_t)x;
  }

  uint32_t operator()() const { return v; }

  ModularInt &operator+=(const ModularInt &other) {
    v += other.v;
    if (v >= M)
      v -= M;
    return *this;
  }

  ModularInt &operator-=(const ModularInt &other) {
    if (v < other.v)
      v += M;
    v -= other.v;
    return *this;
  }

  ModularInt &operator*=(const ModularInt &other) {
    v = (uint32_t)((unsigned long long)v * other.v % M);
    return *this;
  }

  ModularInt power(long long b) const {
    ModularInt res = 1, a = *this;
    while (b > 0) {
      if (b & 1)
        res *= a;
      a *= a;
      b >>= 1;
    }
    return res;
  }

  ModularInt inv() const { return power(M - 2); }

  ModularInt &operator/=(const ModularInt &other) {
    return *this *= other.inv();
  }

  friend ModularInt operator+(ModularInt a, const ModularInt &b) {
    return a += b;
  }
  friend ModularInt operator-(ModularInt a, const ModularInt &b) {
    return a -= b;
  }
  friend ModularInt operator*(ModularInt a, const ModularInt &b) {
    return a *= b;
  }
  friend ModularInt operator/(ModularInt a, const ModularInt &b) {
    return a /= b;
  }

  ModularInt operator-() const { return ModularInt(0) - *this; }
  friend bool operator==(const ModularInt &a, const ModularInt &b) {
    return a.v == b.v;
  }
  friend bool operator!=(const ModularInt &a, const ModularInt &b) {
    return a.v != b.v;
  }
  friend bool operator<(const ModularInt &a, const ModularInt &b) {
    return a.v < b.v;
  }
  friend bool operator>(const ModularInt &a, const ModularInt &b) {
    return a.v > b.v;
  }
  friend bool operator<=(const ModularInt &a, const ModularInt &b) {
    return a.v <= b.v;
  }
  friend bool operator>=(const ModularInt &a, const ModularInt &b) {
    return a.v >= b.v;
  }

  ModularInt &operator++() {
    v++;
    if (v == M)
      v = 0;
    return *this;
  }
  ModularInt operator++(int) {
    ModularInt res = *this;
    ++*this;
    return res;
  }
  ModularInt &operator--() {
    if (v == 0)
      v = M;
    v--;
    return *this;
  }
  ModularInt operator--(int) {
    ModularInt res = *this;
    --*this;
    return res;
  }

  friend std::ostream &operator<<(std::ostream &os, const ModularInt &m) {
    return os << m.v;
  }
  friend std::istream &operator>>(std::istream &is, ModularInt &m) {
    long long x;
    is >> x;
    m = ModularInt(x);
    return is;
  }
};

template <const uint32_t M> ModularInt<M> binpow(ModularInt<M> a, long long b) {
  return a.power(b);
}

} // namespace lib::mod
