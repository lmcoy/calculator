//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <cmath>

#include "State.hpp"
#include "UserFunction.hpp"

using namespace Equation;

NodePtr State::EvalFunction(const std::string &name,
                            const std::list<NodePtr> &x, bool numeric) {
  auto it = funcs.find(name);
  if (it == funcs.end()) {
    std::cerr << "error: unknown function" << name << "\n";
    return 0;
  }
  if (it->second->NumArgs() != x.size()) {
    std::cerr << "error: wrong number of function arguments\n";
    return 0;
  }
  return it->second->Eval(x, numeric);
}

bool State::IsFunction(const std::string &name) {
  auto it = funcs.find(name);
  return !(it == funcs.end());
}

bool State::IsVariable(const std::string &name) {
  auto it = variables.find(name);
  return !(it == variables.end());
}

NumberRepr State::GetVariable(const std::string &name) {
  auto it = variables.find(name);
  if (it == variables.end()) {
    std::cerr << "error: unknown variable " << name << "\n";
    return NumberRepr(-1.0);
  }
  return it->second;
}

DefaultState::DefaultState() {
  funcs["sin"] = std::make_shared<FuncSin>();
  funcs["cos"] = std::make_shared<FuncCos>();

  variables["pi"] = NumberRepr(M_PI);
}
