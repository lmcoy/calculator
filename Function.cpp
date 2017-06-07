//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <algorithm>

#include "Function.hpp"
#include "Number.hpp"
#include "UserFunction.hpp"

using namespace Equation;

void Function::Eval(NodePtr *base, std::shared_ptr<State> state, bool numeric) {
  for (auto &e : args) {
    e->Eval(&e, state, false);
  }

  if (state && state->IsFunction(fname)) {
    auto result = state->EvalFunction(fname, args, numeric);
    if (result) {
      *base = result;
      return;
    }
    if (!numeric) {
      return;
    }
    for (auto &e : args) {
      e->Eval(&e, state, true);
    }
    result = state->EvalFunction(fname, args, true);
    if (result) {
      *base = result;
      return;
    }
  }
}

void Function::writeTreeToStream(std::ostream &s, const std::string &name) {
  std::string type = "fu";
  s << name << "[label=<" << type << ", " << fname << ">]\n";
  int i = 0;
  for (auto it = args.begin(); it != args.end(); it++, i++) {
    std::stringstream ss;
    ss << name << "f" << i;
    s << name << " -> " << ss.str() << ";\n";
  }
  i = 0;
  for (auto it = args.begin(); it != args.end(); it++, i++) {
    std::stringstream ss;
    ss << name << "f" << i;
    (*it)->writeTreeToStream(s, ss.str());
  }
}

void Function::ToStream(std::ostream &s) {
  s << fname;
  s << "(";
  int i = 0;
  for (auto &a : args) {
    if (i != 0) {
      s << ", ";
    }
    a->ToStream(s);
    i += 1;
  }
  s << ")";
}

void Function::ToLatex(std::ostream &s) {
  static std::vector<std::string> funcs = {"exp", "sin"};
  auto isl = std::find(funcs.begin(), funcs.end(), fname);
  if (isl != funcs.end()) {
    s << "\\";
  }
  s << fname;
  s << "\\left(";
  int i = 0;
  for (auto &a : args) {
    if (i != 0) {
      s << ", ";
    }
    a->ToStream(s);
    i += 1;
  }
  s << "\\right)";
}

bool Function::equals(NodePtr n) const {
  if (n->Type() != Node::Type_t::Function) {
    return false;
  }
  auto un = std::static_pointer_cast<Function>(n);
  if (fname != un->fname) {
    return false;
  }
  if (args.size() != un->args.size()) {
    return false;
  }
  auto it1 = args.begin();
  auto it2 = un->args.begin();
  while (it1 != args.end()) {
    if ((*it1)->equals(*it2) == false) {
      return false;
    }
    it1++;
    it2++;
  }
  return true;
}
