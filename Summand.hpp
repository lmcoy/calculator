//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Summand_hpp
#define Summand_hpp

#include "TwoOp.hpp"

namespace Equation {

class Summand : public TwoOp {
public:
  Summand(NodePtr node) : TwoOp(node, "+") {}

  virtual NumberRepr Operation1(NumberRepr base, NumberRepr n) {
    return base + n;
  }

  virtual NumberRepr NeutralElement() { return NumberRepr(0l); }

  virtual enum Type_t Type() const { return Node::Type_t::Summand; }

  virtual void ToStream(std::ostream &s);

  virtual void ToLatex(std::ostream &s);
};
} // namespace Equation

#endif /* Summand_hpp */
