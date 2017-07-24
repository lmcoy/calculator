//
//  Created by Lennart Oymanns on 03.05.17.
//

#include "NumberRepr.hpp"

using namespace Equation;

NumberRepr::NumberRepr() {
  value_double = 0.0;
  isValid = false;
  rational = Rational_t(0);
}

NumberRepr::NumberRepr(const std::string &s) {
  auto dot_pos = s.find_first_of("eE.");
  if (dot_pos != std::string::npos) {
    value_double = strtod(s.c_str(), NULL);
    isFraction = false;
  } else {
    rational = Integer_t(s);
    isFraction = true;
  }
  isValid = true;
}

/** SetFromInt sets the value of this number to fraction num/denom. */
void NumberRepr::SetFromInt(long num, long denom) {
  isFraction = true;
  rational = Rational_t(num, denom);

  value_double = 0.0;
  if (denom == 0) {
    isValid = false;
  } else {
    isValid = true;
  }
}

/** normal *= operator. If *this and rhs are both fractions the result is also a
 * fraction. Otherwise the result is a floating point number. */
NumberRepr &NumberRepr::operator*=(const NumberRepr &rhs) {
  if (!rhs.isValid) {
    isValid = false;
  }
  if (rhs.isFraction && isFraction) {
    rational *= rhs.rational;
    return *this;
  }
  SetFromDouble(Double() * rhs.Double());
  return *this;
}

/** normal /= operator. If *this and rhs are both fractions the result is also a
 * fraction. Otherwise the result is a floating point number. If rhs is a
 * fraction and equal to zero, this number is set to invalid. */
NumberRepr &NumberRepr::operator/=(const NumberRepr &rhs) {
  if (!rhs.isValid) {
    isValid = false;
  }
  if (isFraction && rhs.isFraction) {
    if (rhs.rational == Rational_t(0)) {
      isValid = false;
      return *this;
    }
    rational /= rhs.rational;
    return *this;
  }
  SetFromDouble(Double() / rhs.Double());
  return *this;
}

/** normal += operator. If *this and rhs are both fractions the result is also a
 * fraction. Otherwise the result is a floating point number. */
NumberRepr &NumberRepr::operator+=(const NumberRepr &rhs) {
  if (!rhs.isValid) {
    isValid = false;
  }
  if (rhs.isFraction && isFraction) {
    rational += rhs.rational;
    return *this;
  }
  SetFromDouble(Double() + rhs.Double());
  return *this;
}

/** normal -= operator. If *this and rhs are both fractions the result is also a
 * fraction. Otherwise the result is a floating point number. */
NumberRepr &NumberRepr::operator-=(const NumberRepr &rhs) {
  if (!rhs.isValid) {
    isValid = false;
  }
  if (rhs.isFraction && isFraction) {
    rational -= rhs.rational;
    return *this;
  }
  SetFromDouble(Double() - rhs.Double());
  return *this;
}

std::ostream &operator<<(std::ostream &os, const NumberRepr &obj) {
  os << obj.String();
  return os;
}

/** Pow returns the result of base^exponent.

 If there are any problems (e.g. the exponent is too large), Pow returns an
 invalid number. */
NumberRepr NumberRepr::Pow(const NumberRepr &base, const NumberRepr &exp) {
  if (!base.isValid || !exp.isValid) {
    // power of an invalid number is also invalid.
    auto inv = NumberRepr(0l);
    inv.isValid = false;
    return inv;
  }
  if (!base.IsFraction() || !exp.IsFraction()) {
    // if either base or exponent are floating point numbers, calculate the
    // floating point result.
    double b = base.Double();
    double e = exp.Double();
    return NumberRepr(pow(b, e));
  }
  if (exp.Denominator() == Integer_t(1)) {
    // calculate power if it is not a nth root.
    Integer_t abs_enum = boost::multiprecision::abs(exp.Numerator());
    if (abs_enum > Integer_t(100)) {
      // if the exponent is too large, return a invalid number
      auto inv = NumberRepr(0l);
      inv.isValid = false;
      return inv;
    }

    unsigned e = abs_enum.convert_to<unsigned>();
    auto rat = base.rational;
    Rational_t result_num =
        boost::multiprecision::pow(boost::multiprecision::numerator(rat), e);
    Rational_t result_denom =
        boost::multiprecision::pow(boost::multiprecision::denominator(rat), e);

    if (exp.Numerator() >= Integer_t(0)) {
      Rational_t res = result_num / result_denom;
      return NumberRepr(res);
    }
    Rational_t res = result_denom / result_num;
    return NumberRepr(res);
  }
  // calculate nth root numerically
  auto inv = NumberRepr(pow(base.Double(), exp.Double()));
  inv.isValid = false;
  return inv;
}

/** Double returns a floating point representation of the number. */
double NumberRepr::Double() const {
  if (isFraction) {
    return rational.convert_to<double>();
  }
  return value_double;
}

/** String returns a string representation of the number. */
std::string NumberRepr::String() const {
  if (!isValid) {
    return "nan";
  }
  std::stringstream ss;
  if (isFraction) {
    ss << Numerator().str();
    if (Denominator() != Integer_t(1)) {
      ss << " / " << Denominator().str();
    }
    return ss.str();
  }
  ss << std::setprecision(17) << Double();
  return ss.str();
}

bool NumberRepr::operator==(const NumberRepr &n) const {
  if (!isValid || !n.isValid) {
    return false;
  }
  if (isFraction && n.isFraction) {
    return rational == n.rational;
  }
  if (!isFraction && !n.isFraction) {
    return Double() == n.Double();
  }
  return false;
}

/** ToLatex writes a latex representation of this number to stream s. */
void NumberRepr::ToLatex(std::ostream &s) const {
  if (IsFraction()) {
    if (Denominator() != Integer_t(1)) {
      s << "\\frac{" << Numerator() << "}{" << Denominator() << "}";
    } else {
      s << Numerator();
    }
    return;
  }
  s << std::setprecision(17) << Double();
}

bool NumberRepr::operator<(const NumberRepr &n) {
  if (!isValid || !n.isValid) {
    return false;
  }
  if (isFraction && n.isFraction) {
    return rational < n.rational;
  }
  return Double() < n.Double();
}

bool NumberRepr::operator>(const NumberRepr &n) {
  if (*this < n) {
    return false;
  }
  if (*this == n) {
    return false;
  }
  return true;
}

/** SetFromDouble set the value of this number to the floating  point value l.
 */
void NumberRepr::SetFromDouble(double l) {
  isFraction = false;
  value_double = l;
  isValid = true;
  rational = boost::multiprecision::cpp_rational(0);
}
