//
//  Created by Lennart Oymanns on 03.05.17.
//

#ifndef NumberRepr_hpp
#define NumberRepr_hpp

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

#include <boost/multiprecision/cpp_int.hpp>

namespace Equation {

using Integer_t = boost::multiprecision::cpp_int;
using Rational_t = boost::multiprecision::cpp_rational;

class NumberRepr {
public:
  explicit NumberRepr();
  explicit NumberRepr(const std::string &s);
  explicit NumberRepr(long num, long denom = 1l) { SetFromInt(num, denom); }
  explicit NumberRepr(const Rational_t &rat) {
    isFraction = true;
    isValid = true;
    rational = rat;
  }

  explicit NumberRepr(const Integer_t &rat) {
    isFraction = true;
    isValid = true;
    rational = rat;
  }

  explicit NumberRepr(double l) { SetFromDouble(l); }

  NumberRepr &operator*=(const NumberRepr &rhs);
  NumberRepr &operator/=(const NumberRepr &rhs);
  NumberRepr &operator+=(const NumberRepr &rhs);
  NumberRepr &operator-=(const NumberRepr &rhs);

  void SetFromInt(long num, long denom = 1l);

  void SetFromDouble(double l) {
    isFraction = false;
    value_double = l;
    isValid = true;
    rational = boost::multiprecision::cpp_rational(0);
  }

  bool IsValid() const { return isValid; }
  bool IsFraction() const { return isFraction; }

  double Double() const;
  std::string String() const;
  Integer_t Numerator() const {
    return boost::multiprecision::numerator(rational);
  }
  Integer_t Denominator() const {
    return boost::multiprecision::denominator(rational);
  }

  bool operator==(const NumberRepr &n) const;
  bool operator!=(const NumberRepr &n) const { return !(*this == n); }

  bool operator<(const NumberRepr &n);
  bool operator>(const NumberRepr &n);

  static NumberRepr Pow(const NumberRepr &base, const NumberRepr &exp);

  void ToLatex(std::ostream &s) const;

private:
  double value_double;
  bool isFraction = false;
  bool isValid = true;

  Rational_t rational;
};

inline NumberRepr operator+(NumberRepr lhs, const NumberRepr &rhs) {
  lhs += rhs;
  return lhs;
}

inline NumberRepr operator*(NumberRepr lhs, const NumberRepr &rhs) {
  lhs *= rhs;
  return lhs;
}

inline NumberRepr operator-(NumberRepr lhs, const NumberRepr &rhs) {
  lhs -= rhs;
  return lhs;
}

inline NumberRepr operator/(NumberRepr lhs, const NumberRepr &rhs) {
  lhs /= rhs;
  return lhs;
}

} // namespace Equation

std::ostream &operator<<(std::ostream &os, const Equation::NumberRepr &obj);

#endif /* NumberRepr_hpp */
