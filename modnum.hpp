#pragma once
#include <iostream>

template <class T, class AddT, class MultT, T P>
struct ModNum {
  T val = 0;
  constexpr ModNum() = default;
  constexpr ModNum(T x) : val(x) {}
  template <class S> static constexpr ModNum Safe(S x) { return static_cast<T>( ((x%P)+P)%P ); }
  explicit constexpr operator T() const { return val; }

  constexpr bool operator==(ModNum rhs) const { return val == rhs.val; }
  constexpr bool operator!=(ModNum rhs) const { return val != rhs.val; }
  constexpr ModNum operator-() const { return (val == 0) ? (*this) : (P - val); }

  constexpr ModNum& operator+=(ModNum rhs) { return *this = Add(val, rhs.val); }
  constexpr ModNum& operator-=(ModNum rhs) { return *this = Sub(val, rhs.val); }
  constexpr ModNum& operator*=(ModNum rhs) { return *this = Mult(val, rhs.val); }
  constexpr ModNum& operator/=(ModNum rhs) { return *this *= rhs.Inverse(); }
  constexpr ModNum& operator++() { return *this += 1; }
  constexpr ModNum operator++(int) { ModNum old(*this); ++*this; return old; }
  constexpr ModNum& operator--() { return *this -= 1; }
  constexpr ModNum operator--(int) { ModNum old(*this); ++*this; return old; }

  template <class S>
  constexpr ModNum Pow(S ex) const {
    if (P == 1) return 0;
    if (ex < 0) return Pow(-ex).Inverse();
    ModNum result = 1;
    ModNum base(*this);
    while (ex > 0) {
      if (ex & 1) result *= base;
      ex >>= 1;
      base *= base;
    }
    return result;
  }
  constexpr ModNum Inverse() const { return Pow(P-2); }
  friend constexpr ModNum operator+(ModNum lhs, ModNum rhs) { return Add(lhs.val, rhs.val); }
  friend constexpr ModNum operator-(ModNum lhs, ModNum rhs) { return Sub(lhs.val, rhs.val); }
  friend constexpr ModNum operator*(ModNum lhs, ModNum rhs) { return Mult(lhs.val, rhs.val); }
  friend constexpr ModNum operator/(ModNum lhs, ModNum rhs) { return lhs * rhs.Inverse(); }

  private:
  static constexpr T Add(AddT lhs, AddT rhs) {
    auto sum = lhs+rhs;
    return (sum < P) ? sum : (sum - P);
  }
  static constexpr T Sub(AddT lhs, AddT rhs) {
    auto dif = lhs-rhs;
    return (dif >= 0) ? dif : (dif + P);
  }
  static constexpr T Mult(MultT lhs, MultT rhs) {
    return (lhs*rhs) % P;
  }  
  friend std::ostream& operator<<(std::ostream& os, ModNum m) { return os << m.val; }
  friend std::istream& operator>>(std::istream& is, ModNum& m) { is >> m.val; return is; }
};
using Num = ModNum<int, int, long long, 1000000007>;