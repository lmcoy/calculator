//
//  NumberRepr.cpp
//  Project
//
//  Created by Lennart Oymanns on 03.05.17.
//
//

#include "NumberRepr.hpp"

using namespace Equation;

namespace {
long gcd(long a, long b) {
  while (b != 0) {
    long h = a % b;
    a = b;
    b = h;
  }
  return a;
}
} // namespace

NumberRepr::NumberRepr() {
  value_double = 0.0;
  value_num = 0l;
  value_denom = 1l;
  isValid = false;
}
NumberRepr::NumberRepr(const std::string &s) {
  value_double = strtod(s.c_str(), NULL);

  char *end;
  value_num = strtol(s.c_str(), &end, 10);
  value_denom = 1l;

  if (*end == '\0') {
    isFraction = true;
  }
}

void NumberRepr::SetFromInt(long num, long denom) {
  isFraction = true;
  bool negative = false;
  if (num < 0l) {
    negative = true;
    num *= -1l;
  }
  if (denom < 0) {
    if (negative) {
      negative = false;
    } else {
      negative = true;
    }
    denom *= -1l;
  }

  long g = gcd(num, denom);
  value_num = num / g;
  value_denom = denom / g;

  value_double = static_cast<double>(num) / static_cast<double>(denom);
  if (negative) {
    value_num *= -1l;
    value_double *= -1.0;
  }
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
    SetFromInt(value_num * rhs.value_num, value_denom * rhs.value_denom);
    return *this;
  }
  SetFromDouble(value_double * rhs.value_double);
  return *this;
}

NumberRepr &NumberRepr::operator/=(const NumberRepr &rhs) {
  if (!rhs.isValid) {
    isValid = false;
  }
  if (isFraction && rhs.isFraction) {
    if (rhs.value_num == 0l) {
      isValid = false;
      return *this;
    }
    SetFromInt(value_num * rhs.value_denom, value_denom * rhs.value_num);
    return *this;
  }
  SetFromDouble(value_double / rhs.value_double);
  return *this;
}

NumberRepr &NumberRepr::operator+=(const NumberRepr &rhs) {
  if (!rhs.isValid) {
    isValid = false;
  }
  if (rhs.isFraction && isFraction) {
    long denom = value_denom * rhs.value_denom;
    long num = value_num * rhs.value_denom + rhs.value_num * value_denom;
    SetFromInt(num, denom);
    return *this;
  }
  SetFromDouble(value_double + rhs.value_double);
  return *this;
}

NumberRepr &NumberRepr::operator-=(const NumberRepr &rhs) {
  if (!rhs.isValid) {
    isValid = false;
  }
  if (rhs.isFraction && isFraction) {
    long denom = value_denom * rhs.value_denom;
    long num = value_num * rhs.value_denom - rhs.value_num * value_denom;
    SetFromInt(num, denom);
    return *this;
  }
  SetFromDouble(value_double - rhs.value_double);
  return *this;
}

std::ostream &operator<<(std::ostream &os, const NumberRepr &obj) {
  os << obj.String();
  return os;
}

NumberRepr NumberRepr::Pow(const NumberRepr &base, const NumberRepr &exp) {
  if (!base.isValid || !exp.isValid) {
    auto inv = NumberRepr(pow(base.Double(), exp.Double()));
    inv.isValid = false;
    return inv;
  }
  if (!base.IsFraction() || !exp.IsFraction()) {
    return NumberRepr(pow(base.Double(), exp.Double()));
  }
  if (base.IsFraction() && exp.value_denom == 1l) {
    long abs_enum = std::abs(exp.value_num);
    long result_num = 1l;
    long result_denom = 1l;
    long expo = abs_enum;
    long b_n = base.value_num;
    long b_d = base.value_denom;
    while (expo) {
      if (expo & 1) {
        result_num *= b_n;
        result_denom *= b_d;
      }
      expo >>= 1;
      b_n *= b_n;
      b_d *= b_d;
    }
    if (exp.value_num >= 0) {
      return NumberRepr(result_num, result_denom);
    }
    return NumberRepr(result_denom, result_num);
  }
  auto inv = NumberRepr(pow(base.Double(), exp.Double()));
  inv.isValid = false;
  return inv;
}
