//
//  NumberRepr.cpp
//  Project
//
//  Created by Lennart Oymanns on 03.05.17.
//
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

NumberRepr NumberRepr::Pow(const NumberRepr &base, const NumberRepr &exp) {
  if (!base.isValid || !exp.isValid) {
    auto inv = NumberRepr(0l);
    inv.isValid = false;
    return inv;
  }
  if (!base.IsFraction() || !exp.IsFraction()) {
    double b = base.Double();
    double e = exp.Double();
    return NumberRepr(pow(b, e));
  }
  if (base.IsFraction() && exp.Denominator() == Integer_t(1)) {
    Integer_t abs_enum = boost::multiprecision::abs(exp.Numerator());
    if (abs_enum > Integer_t(100)) { 
      // if the exponent is too large, return a invalid number
      auto inv = NumberRepr(0l);
      inv.isValid = false;
      return inv;
    }

    unsigned e = abs_enum.template convert_to<unsigned>();
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
  auto inv = NumberRepr(pow(base.Double(), exp.Double()));
  inv.isValid = false;
  return inv;
}

double NumberRepr::Double() const {
  if (isFraction) {
    return rational.convert_to<double>();
  }
  return value_double;
}

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

void NumberRepr::ToLatex(std::ostream &s) const {
  if (IsFraction()) {
    if (Denominator() != Integer_t(1)) {
      s << "\\frac{" << Numerator() << "}{" << Denominator() << "}";
    } else {
      s << Numerator();
    }
    return;
  }
  s << Double();
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
