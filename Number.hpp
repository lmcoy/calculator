//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Number_hpp
#define Number_hpp

#include <iomanip>

#include "Node.hpp"
#include "NumberRepr.hpp"

namespace Equation {

class Number : public Node {
public:
  explicit Number(const std::string &number) : value(number) {}

  explicit Number(double v) : value(v) {}

  explicit Number(long v) : value(v) {}

  explicit Number(const NumberRepr &n) : value(n) {}

  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false) {
    if (numeric) {
      value.SetFromDouble(value.Double());
    }
  }

  virtual enum Type Type() const { return Node::Type::Number; }

  NumberRepr GetValue() const { return value; }

  virtual void ToStream(std::ostream &s) { s << value; }

  virtual void writeTreeToStream(std::ostream &s, const std::string &name);

  virtual bool IsNumber() const { return true; }

  virtual bool equals(NodePtr n) const {
    if (n->Type() != Node::Type::Number) {
      return false;
    }
    auto number = std::static_pointer_cast<Number>(n);

    return number && value == number->value;
  }

  virtual void ToLatex(std::ostream &s) { value.ToLatex(s); }

private:
  NumberRepr value;
};
} // namespace Equation

#endif /* Number_hpp */
