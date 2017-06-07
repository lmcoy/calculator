//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#include "State.hpp"

namespace Equation {

class Node;

typedef std::shared_ptr<Node> NodePtr;

class Node {
public:
  enum class Type {
    Number,
    Summand,
    Factor,
    UnaryMinus,
    Variable,
    Power,
    Function,
  };
  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false) = 0;

  virtual Type Type() const = 0;
  virtual ~Node() {}

  virtual std::string ToString() {
    std::stringstream s;
    ToStream(s);
    return s.str();
  }

  virtual void ToLatex(std::ostream &s) = 0;

  virtual bool IsNumber() const = 0;

  virtual void ToStream(std::ostream &s) = 0;

  virtual void WriteTreeToStream(std::ostream &s) {
    s << "digraph g {\n";
    writeTreeToStream(s, "n");
    s << "}\n";
  }

  virtual void writeTreeToStream(std::ostream &s, const std::string &name) = 0;

  bool operator==(const NodePtr n) const { return equals(n); }

  virtual bool equals(NodePtr n) const = 0;
};
} // namespace Equation

#endif /* Node_hpp */
