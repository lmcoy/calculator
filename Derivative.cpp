#include "Derivative.hpp"

#include "Error.h"
#include "Factor.hpp"
#include "Function.hpp"
#include "Node.hpp"
#include "Number.hpp"
#include "Parser.hpp"
#include "Power.hpp"
#include "Summand.hpp"
#include "UnaryMinus.hpp"
#include "Variable.hpp"

namespace Equation {

Derivative::Derivative() {
  Parser parser;
  functions["sin"] = parser.Parse("cos(x_)");
  functions["cos"] = parser.Parse("-sin(x_)");
  functions["tan"] = parser.Parse("1/(cos(x_))^2");
  functions["exp"] = parser.Parse("exp(x_)");
  functions["log"] = parser.Parse("1/x_");
  functions["asin"] = parser.Parse("1/(1-x_^2)^(1/2)");
  functions["acos"] = parser.Parse("-1/(1-x_^2)^(1/2)");
  functions["atan"] = parser.Parse("1/(1+x_^2)");
  functions["sinh"] = parser.Parse("cosh(x_)");
  functions["cosh"] = parser.Parse("sinh(x_)");
  functions["tanh"] = parser.Parse("1/(cosh(x_))^2");
}

NodePtr Derivative::Eval(const std::list<NodePtr> &args, bool numeric) {
  if (args.size() != 2) {
    throw InputError(0, "expected two arguments for function 'D'.");
  }
  auto it = args.begin();
  auto func = *it;
  std::advance(it, 1);
  auto var = *it;

  if (var->Type() != Node::Type_t::Variable) {
    throw InputError(0, "expected variable as 2nd argument of 'D'");
  }

  if (func->Type() == Node::Type_t::Number) {
    return Dnumber(std::static_pointer_cast<Number>(func));
  }
  auto varname = std::static_pointer_cast<Variable>(var)->Name();
  if (func->Type() == Node::Type_t::Variable) {
    return Dvariable(std::static_pointer_cast<Variable>(func), varname);
  }

  if (func->Type() == Node::Type_t::Summand) {
    return Dsum(std::static_pointer_cast<Summand>(func), varname);
  }

  if (func->Type() == Node::Type_t::Power) {
    return Dpower(std::static_pointer_cast<Power>(func), varname);
  }

  if (func->Type() == Node::Type_t::UnaryMinus) {
    return Dminus(std::static_pointer_cast<UnaryMinus>(func), varname);
  }

  if (func->Type() == Node::Type_t::Factor) {
    return Dmul(std::static_pointer_cast<Factor>(func), varname);
  }

  if (func->Type() == Node::Type_t::Function) {
    return Dfunc(std::static_pointer_cast<Function>(func), varname);
  }

  return 0;
}

NodePtr Derivative::Dnumber(const NumberPtr &nb) {
  return std::make_shared<Number>(0l);
}

NodePtr Derivative::Dvariable(const VariablePtr &v, const std::string &var) {
  if (v->Name() == var) {
    return std::make_shared<Number>(1l);
  }
  return std::make_shared<Number>(0l);
}

NodePtr Derivative::Dsum(const SummandPtr &v, const std::string &var) {
  auto newsum = std::make_shared<Summand>();

  for (const auto &s : v->Data()) {
    std::list<NodePtr> args;
    args.push_back(s);
    args.push_back(std::make_shared<Variable>(var));
    NodePtr D = std::make_shared<Function>("D", args);
    newsum->Data().push_back(D);
  }

  NodePtr result = std::static_pointer_cast<Node>(newsum);
  result->Eval(&result, std::make_shared<DefaultState>());
  return result;
}

NodePtr Derivative::Dpower(const PowerPtr &v, const std::string &var) {
  auto root = std::make_shared<Factor>();

  auto sum1 = std::make_shared<Summand>(v->Exponent()->clone());
  sum1->AddOp1(std::make_shared<Number>(-1l));
  auto p1 = std::make_shared<Power>(v->Base()->clone(), sum1);

  root->AddOp1(p1);

  auto mul1 = std::make_shared<Factor>();
  mul1->AddOp1(v->Exponent()->clone());
  std::list<NodePtr> args1 = {
      {v->Base()->clone(), std::make_shared<Variable>(var)}};
  mul1->AddOp1(std::make_shared<Function>("D", args1));

  auto mul2 = std::make_shared<Factor>();
  mul2->AddOp1(v->Base()->clone());
  std::list<NodePtr> args2 = {
      {v->Exponent()->clone(), std::make_shared<Variable>(var)}};
  mul2->AddOp1(std::make_shared<Function>("D", args2));
  std::list<NodePtr> args3 = {
      {v->Base()->clone(), std::make_shared<Variable>(var)}};
  mul2->AddOp1(std::make_shared<Function>("D", args3));

  auto sum2 = std::make_shared<Summand>();
  sum2->AddOp1(mul1);
  sum2->AddOp1(mul2);

  root->AddOp1(sum2);

  NodePtr result = std::static_pointer_cast<Node>(root);
  result->Eval(&result, std::make_shared<DefaultState>());
  return result;
}

NodePtr Derivative::Dminus(const UnaryMinusPtr &v, const std::string &var) {
  std::list<NodePtr> args = {v->Data(), std::make_shared<Variable>(var)};
  NodePtr result =
      std::make_shared<UnaryMinus>(std::make_shared<Function>("D", args));
  result->Eval(&result, std::make_shared<DefaultState>());
  return result;
}

NodePtr Derivative::Dmul(const FactorPtr &v, const std::string &var) {
  auto sum = std::make_shared<Summand>();
  for (size_t i = 0; i < v->Data().size(); i++) {
    auto mul = std::make_shared<Factor>();
    for (size_t j = 0; j < v->Data().size(); j++) {
      auto t = v->Data().begin();
      std::advance(t, j);
      if (j != i) {
        mul->AddOp1((*t)->clone());
      } else {
        std::list<NodePtr> args = {(*t)->clone(),
                                   std::make_shared<Variable>(var)};
        mul->AddOp1(std::make_shared<Function>("D", args));
      }
    }
    sum->AddOp1(mul);
  }
  NodePtr result = std::static_pointer_cast<Node>(sum);
  result->Eval(&result, std::make_shared<DefaultState>());
  return result;
}

NodePtr Derivative::Dfunc(const FunctionPtr &v, const std::string &var) {
  auto it = functions.find(v->Name());
  if (it == functions.end()) {
    return 0;
  }

  auto factor = std::make_shared<Factor>();
  if (v->Args().size() != 1) {
    return 0;
  }
  auto arg = v->Args().begin();

  auto state = std::make_shared<DefaultState>();
  state->SetVariable("x_", (*arg)->clone());

  Parser parser;
  // auto outer = parser.Parse("cos(x_)");
  auto outer = it->second->clone();
  factor->AddOp1(outer);

  std::list<NodePtr> args = {*arg, std::make_shared<Variable>(var)};
  auto inner = std::make_shared<Function>("D", args);

  factor->AddOp1(inner);

  NodePtr result = std::static_pointer_cast<Node>(factor);
  result->Eval(&result, state);
  return result;
}

} // namespace Equation
