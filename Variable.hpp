//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Variable_hpp
#define Variable_hpp

#include <string>

#include "Number.hpp"

namespace Equation {

class Variable : public Node {
public:
  explicit Variable(const std::string &n) : vname(n) {}

  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false) {
    if (numeric && state->IsVariable(vname)) {
      auto result = state->GetVariable(vname);
      base->reset(new Number(result));
    }
  }

  virtual enum Type_t Type() const { return Node::Type_t::Variable; }

  virtual void ToStream(std::ostream &s) { s << vname; }

  virtual void ToLatex(std::ostream &s) { s << vname; }

  virtual void writeTreeToStream(std::ostream &s, const std::string &name);

  virtual bool IsNumber() const { return false; }

  virtual bool equals(NodePtr n) const {
    if (n->Type() != Node::Type_t::Variable) {
      return false;
    }
    auto un = std::static_pointer_cast<Variable>(n);
    return un && vname == un->vname;
  }

  virtual NodePtr clone() const { return std::make_shared<Variable>(vname); }

private:
  std::string vname;
};
} // namespace Equation

#endif /* Variable_hpp */
