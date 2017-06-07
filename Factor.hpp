//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Factor_hpp
#define Factor_hpp

#include "TwoOp.hpp"

namespace Equation {

class Factor : public TwoOp {
public:
  Factor(NodePtr node) : TwoOp(node, "*") {}

  virtual NumberRepr Operation1(NumberRepr base, NumberRepr n) {
    return base * n;
  }

  virtual NumberRepr NeutralElement() { return NumberRepr(1l); }

  virtual enum Type_t Type() const { return Node::Type_t::Factor; }

  virtual void ToStream(std::ostream &s);

  virtual void ToLatex(std::ostream &os);

  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false);

  bool HasFactor(NodePtr f) { return false; }

private:
  void simplify(std::list<NodePtr> &op, std::shared_ptr<State> state);
};

} // namespace Equation

#endif /* Factor_hpp */
