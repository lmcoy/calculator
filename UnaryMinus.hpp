//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef UnaryMinus_hpp
#define UnaryMinus_hpp

#include "Node.hpp"
#include <ostream>

namespace Equation {

class UnaryMinus : public Node {
public:
  UnaryMinus(NodePtr node) : data(node) {}

  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false);

  virtual enum Type Type() const { return Node::Type::UnaryMinus; }

  virtual void ToStream(std::ostream &s) {
    s << "-";
    data->ToStream(s);
  }

  virtual void ToLatex(std::ostream &s) {
    s << "-";
    data->ToLatex(s);
  }

  virtual void writeTreeToStream(std::ostream &s, const std::string &name);

  virtual bool IsNumber() const { return data->IsNumber(); }

  virtual bool equals(NodePtr n) const {
    if (n->Type() != Node::Type::UnaryMinus) {
      return false;
    }
    auto un = std::static_pointer_cast<UnaryMinus>(n);
    return data->equals(un->data);
  }

private:
  NodePtr data;
};
} // namespace Equation

#endif /* UnaryMinus_hpp */