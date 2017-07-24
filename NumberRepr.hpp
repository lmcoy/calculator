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

/** NumberRepr represents an actual number.

 A number is either a fraction with denominator and numerator (with arbitrary
 precision) or it is a floating point number.
 */
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

  explicit NumberRepr(const Integer_t &i) {
    isFraction = true;
    isValid = true;
    rational = i;
  }

  explicit NumberRepr(double l) { SetFromDouble(l); }

  NumberRepr &operator*=(const NumberRepr &rhs);

  NumberRepr &operator/=(const NumberRepr &rhs);

  NumberRepr &operator+=(const NumberRepr &rhs);

  NumberRepr &operator-=(const NumberRepr &rhs);

  void SetFromInt(long num, long denom = 1l);

  void SetFromDouble(double l);

  bool IsValid() const { return isValid; }

  /** IsFraction returns true if this number is a fraction. */
  bool IsFraction() const { return isFraction; }

  double Double() const;

  std::string String() const;

  /** Numerator returns the numerator of the fraction.

   Note, the result is only useful if IsFraction() is true. */
  Integer_t Numerator() const {
    return boost::multiprecision::numerator(rational);
  }

  /** Denominator returns the denominator of the fraction.

   Note, the result is only useful if IsFraction() is true. */
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
  double value_double;     ///< if isFraction==false, the value of this number
  bool isFraction = false; ///< determines if this number is a fration or not
  bool isValid = true;     ///< determines if the number is valid
  Rational_t rational;     ///< if isFraction==true, the value of this number
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
