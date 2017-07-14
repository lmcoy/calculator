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

  virtual enum Type_t Type() const { return Node::Type_t::Power; }

  virtual void ToLatex(std::ostream &s);
  virtual void ToStream(std::ostream &s);

  virtual void writeTreeToStream(std::ostream &s, const std::string &name);

  virtual bool IsNumber() const {
    return base->IsNumber() && exponent->IsNumber();
  }

  virtual bool equals(NodePtr n) const;

  NodePtr Base() { return base; }
  NodePtr Exponent() { return exponent; }

  virtual NodePtr clone() const {
    return std::make_shared<Power>(base->clone(), exponent->clone());
  }

private:
  NodePtr base;
  NodePtr exponent;
};
} // namespace Equation

#endif /* Power_hpp */
