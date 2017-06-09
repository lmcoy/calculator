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

namespace Equation {

class NumberRepr {
public:
  explicit NumberRepr();
  explicit NumberRepr(const std::string &s);
  explicit NumberRepr(long num, long denom = 1l) { SetFromInt(num, denom); }

  explicit NumberRepr(double l) { SetFromDouble(l); }

  NumberRepr &operator*=(const NumberRepr &rhs);
  NumberRepr &operator/=(const NumberRepr &rhs);
  NumberRepr &operator+=(const NumberRepr &rhs);
  NumberRepr &operator-=(const NumberRepr &rhs);

  void SetFromInt(long num, long denom = 1l);

  void SetFromDouble(double l) {
    isFraction = false;
    value_num = (long)l;
    value_denom = 1l;
    value_double = l;
    isValid = true;
  }

  bool IsValid() const { return isValid; }
  bool IsFraction() const { return isFraction; }

  double Double() const { return value_double; }
  std::string String() const {
    if (!isValid) {
      return "nan";
    }
    std::stringstream ss;
    if (isFraction) {
      ss << value_num;
      if (value_denom != 1l) {
        ss << " / " << value_denom;
      }
      return ss.str();
    }
    ss << std::setprecision(17) << value_double;
    return ss.str();
  }

  long Numerator() const { return value_num; }
  long Denominator() const { return value_denom; }

  bool operator==(const NumberRepr &n) const {
    if (!isValid || !n.isValid) {
      return false;
    }
    if (isFraction && n.isFraction) {
      return value_num == n.value_num && value_denom == n.value_denom;
    }
    if (!isFraction && !n.isFraction) {
      return value_double == n.value_double;
    }
    return false;
  }
  bool operator!=(const NumberRepr &n) const { return !(*this == n); }

  bool operator<(const NumberRepr &n) {
    if (!isValid || !n.isValid) {
      return false;
    }
    if (isFraction && n.isFraction) {
      auto a = value_num * n.value_denom;
      auto b = n.value_num * value_denom;
      return a < b;
    }
    return value_double < n.value_double;
  }

  bool operator>(const NumberRepr &n) {
    if (*this < n) {
      return false;
    }
    if (*this == n) {
      return false;
    }
    return true;
  }

  static NumberRepr Pow(const NumberRepr &base, const NumberRepr &exp);

  void ToLatex(std::ostream &s) const {
    if (IsFraction()) {
      if (value_denom != 1l) {
        s << "\\frac{" << value_num << "}{" << value_denom << "}";
      } else {
        s << value_num;
      }
      return;
    }
    s << value_double;
  }

private:
  long value_num, value_denom;
  double value_double;
  bool isFraction = false;
  bool isValid = true;
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
