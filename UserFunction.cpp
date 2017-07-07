//
//  Created by Lennart Oymanns on 03.06.17.
//
#include <cmath>

#include "Error.h"
#include "Number.hpp"
#include "Parser.hpp"
#include "UserFunction.hpp"

using namespace Equation;

NodePtr UserFunction::Eval(const std::list<NodePtr> &args, bool numeric) {

  NodePtr result;
  bool rep = SpecialValues(args, &result);
  if (rep) {
    return result;
  }

  std::vector<NumberRepr> dargs;
  dargs.reserve(args.size());

  bool num = true;
  bool real_values = true;
  for (const auto &a : args) {
    if (a->Type() == Node::Type_t::Number) {
      const auto n = std::static_pointer_cast<Number>(a);
      if (n->GetValue().IsFraction()) {
        real_values = false;
      }
      dargs.push_back(n->GetValue());
    } else {
      num = false;
      break;
    }
  }

  if (num) {
    if (numeric || real_values) {
      return EvalNum(dargs);
    }
  }

  return 0;
}

NodePtr UserFunction::make_node(const std::string &expr) {
  Parser parser;
  NodePtr node;
  try {
    node = parser.Parse(expr);
  } catch (InputError &e) {
    return 0;
  }
  auto state = std::make_shared<State>();
  node->Eval(&node, state);
  return node;
}

NodePtr FuncSin::EvalNum(const std::vector<NumberRepr> &args) {
  return std::make_shared<Number>(sin(args[0].Double()));
}

std::list<std::pair<NodePtr, std::string>> FuncSin::svalues = {
    {UserFunction::make_node("0"), "0"},
    {UserFunction::make_node("pi"), "0"},
    {UserFunction::make_node("2*pi"), "0"},
    {UserFunction::make_node("1/2*pi"), "1"},
    {UserFunction::make_node("3*pi/2"), "-1"}};

bool FuncSin::SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}

// --- cos --------------

NodePtr FuncCos::EvalNum(const std::vector<NumberRepr> &args) {
  return std::make_shared<Number>(cos(args[0].Double()));
}

std::list<std::pair<NodePtr, std::string>> FuncCos::svalues = {
    {UserFunction::make_node("0"), "1"},
    {UserFunction::make_node("pi"), "-1"},
    {UserFunction::make_node("2*pi"), "1"},
    {UserFunction::make_node("1/2*pi"), "0"},
    {UserFunction::make_node("3*pi/2"), "0"}};

bool FuncCos::SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}
