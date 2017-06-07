//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "NumberRepr.hpp"

namespace Equation {

class UserFunction;
class Node;
typedef std::shared_ptr<Node> NodePtr;

class State {
public:
  bool IsFunction(const std::string &name);
  bool IsVariable(const std::string &name);

  NodePtr EvalFunction(const std::string &name, const std::list<NodePtr> &x,
                       bool numeric);
  NumberRepr GetVariable(const std::string &name);

  void SetVariable(const std::string &name, NumberRepr value) {
    variables[name] = value;
  }

protected:
  std::map<std::string, std::shared_ptr<UserFunction>> funcs;
  std::map<std::string, NumberRepr> variables;
};

class DefaultState : public State {
public:
  DefaultState();
};

typedef std::shared_ptr<State> StatePtr;
} // namespace Equation

#endif /* State_hpp */
