//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <cmath>

#include "Derivative.hpp"
#include "Error.h"
#include "Number.hpp"
#include "State.hpp"
#include "UserFunction.hpp"

using namespace Equation;

NodePtr State::EvalFunction(const std::string &name,
                            const std::list<NodePtr> &x, bool numeric) {
  auto it = funcs.find(name);
  if (it == funcs.end()) {
    return 0;
  }
  if (it->second->NumArgs() != x.size()) {
    throw InputError(0, "wrong number of arguments");
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

NodePtr State::GetVariable(const std::string &name) {
  auto it = variables.find(name);
  if (it == variables.end()) {
    throw InputError(0, "unknown variable");
  }
  return it->second;
}

DefaultState::DefaultState() {
  funcs["sin"] = std::make_shared<FuncSin>();
  funcs["cos"] = std::make_shared<FuncCos>();
  funcs["tan"] = std::make_shared<FuncTan>();
  funcs["asin"] = std::make_shared<FuncASin>();
  funcs["acos"] = std::make_shared<FuncACos>();
  funcs["atan"] = std::make_shared<FuncATan>();
  funcs["exp"] = std::make_shared<FuncExp>();
  funcs["D"] = std::make_shared<Derivative>();

  variables["pi"] = std::make_shared<Number>(M_PI);
}
