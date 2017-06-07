//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Power_hpp
#define Power_hpp

#include "Node.hpp"

namespace Equation {

class Power : public Node {
public:
  Power(NodePtr b, NodePtr exp) : base(b), exponent(exp) {}

  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false);

  virtual enum Type Type() const { return Node::Type::Power; }

  virtual void ToStream(std::ostream &s) {
    bool brackets_b = false;
    if (base->Type() == Node::Type::Factor ||
        base->Type() == Node::Type::Power ||
        base->Type() == Node::Type::Summand) {
      brackets_b = true;
    }
    bool brackets_e = false;
    if (exponent->Type() == Node::Type::Factor ||
        exponent->Type() == Node::Type::Power ||
        exponent->Type() == Node::Type::Summand) {
      brackets_e = true;
    }

    if (brackets_b) {
      s << "(";
    }
    base->ToStream(s);
    if (brackets_b) {
      s << ")";
    }
    s << " ^ ";
    if (brackets_e) {
      s << "(";
    }
    exponent->ToStream(s);
    if (brackets_e) {
      s << ")";
    }
  }

  virtual void ToLatex(std::ostream &s) {
    bool brackets_b = false;
    if (base->Type() == Node::Type::Factor ||
        base->Type() == Node::Type::Power ||
        base->Type() == Node::Type::Summand) {
      brackets_b = true;
    }
    bool brackets_e = false;
    if (exponent->Type() == Node::Type::Factor ||
        exponent->Type() == Node::Type::Power ||
        exponent->Type() == Node::Type::Summand) {
      brackets_e = true;
    }
    if (exponent->Type() == Node::Type::Number) {
      auto number = std::static_pointer_cast<Number>(exponent);
      if (number->GetValue() < NumberRepr(0l)) {
        brackets_e = true;
      }
    }

    if (brackets_b) {
      s << "\\left(";
    }
    base->ToStream(s);
    if (brackets_b) {
      s << "\\right)";
    }
    s << "^";
    if (brackets_e) {
      s << "{";
    }
    exponent->ToStream(s);
    if (brackets_e) {
      s << "}";
    }
  }

  virtual void writeTreeToStream(std::ostream &s, const std::string &name);

  virtual bool IsNumber() const {
    return base->IsNumber() && exponent->IsNumber();
  }

  virtual bool equals(NodePtr n) const {
    if (n->Type() != Node::Type::Power) {
      return false;
    }
    auto un = std::static_pointer_cast<Power>(n);
    return base->equals(un->base) && exponent->equals(un->exponent);
  }

  NodePtr Base() { return base; }
  NodePtr Exponent() { return exponent; }

private:
  NodePtr base;
  NodePtr exponent;
};
} // namespace Equation

#endif /* Power_hpp */
