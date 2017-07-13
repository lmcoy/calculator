//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Function_hpp
#define Function_hpp

#include "Node.hpp"

namespace Equation {

class Function : public Node {
public:
  Function(std::string n) : fname(n) {}

  Function(const std::string &n, const std::list<NodePtr> a) {
    fname = n;
    for (auto &e : a) {
      args.push_back(e->clone());
    }
  }

  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false);

  void AddArg(NodePtr ptr) { args.push_back(ptr); }

  virtual enum Type_t Type() const { return Node::Type_t::Function; }

  virtual void ToStream(std::ostream &s);

  virtual void ToLatex(std::ostream &s);

  virtual void writeTreeToStream(std::ostream &s, const std::string &name);

  virtual bool IsNumber() const { return false; }

  virtual bool equals(NodePtr n) const;

  virtual NodePtr clone() const {
    return std::make_shared<Function>(fname, args);
  }

  virtual std::string Name() const { return fname; }
  virtual const std::list<NodePtr> &Args() const { return args; }

private:
  std::list<NodePtr> args;
  std::string fname;
};
} // namespace Equation

#endif /* Function_hpp */
