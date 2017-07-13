//
//  Created by Lennart Oymanns on 03.06.17.
//
#include <cmath>
#include <complex>

#include "Error.h"
#include "Factor.hpp"
#include "Function.hpp"
#include "Number.hpp"
#include "NumberRepr.hpp"
#include "Parser.hpp"
#include "Power.hpp"
#include "State.hpp"
#include "Summand.hpp"
#include "UnaryMinus.hpp"
#include "UserFunction.hpp"
#include "Variable.hpp"

using namespace Equation;

static NodePtr node_from_complex(std::complex<double> c) {
  if (c.imag() == 0.0) {
    return std::make_shared<Number>(c.real());
  }
  if (c.real() == 0.0) {
    auto factor = std::make_shared<Factor>();
    factor->AddOp1(std::make_shared<Number>(c.imag()));
    factor->AddOp1(std::make_shared<Variable>("i"));
    return factor;
  }
  auto real = node_from_complex(std::complex<double>(c.real(), 0.0));
  auto imag = node_from_complex(std::complex<double>(0.0, c.imag()));
  auto sum = std::make_shared<Summand>();
  sum->AddOp1(real);
  sum->AddOp1(imag);
  return sum;
}

static std::pair<std::complex<NumberRepr>, bool> complex_number(NodePtr n) {
  using ret_t = std::pair<std::complex<NumberRepr>, bool>;
  static const ret_t invalid(std::complex<NumberRepr>(), false);
  // is real?
  if (n->Type() == Node::Type_t::Number) {
    auto nb = std::static_pointer_cast<Number>(n);
    auto value = std::complex<NumberRepr>(nb->GetValue(), NumberRepr(0l));
    return ret_t(value, true);
  }
  // is imaginary?
  if (n->Type() == Node::Type_t::Variable) {
    auto var = std::static_pointer_cast<Variable>(n);
    if (var->Name() == "i") {
      auto value = std::complex<NumberRepr>(NumberRepr(0l), NumberRepr(1l));
      return ret_t(value, true);
    }
    return invalid;
  }
  if (n->Type() == Node::Type_t::Factor) {
    auto factor = std::static_pointer_cast<Factor>(n);
    if (factor->Data().size() != 2) {
      return invalid;
    }
    auto it = factor->Data().begin();
    if ((*it)->Type() != Node::Type_t::Number) {
      return invalid;
    }
    auto nb = std::static_pointer_cast<Number>(*it);
    std::advance(it, 1);
    if ((*it)->Type() != Node::Type_t::Variable) {
      return invalid;
    }
    auto var = std::static_pointer_cast<Variable>(*it);
    if (var->Name() != "i") {
      return invalid;
    }
    auto value = std::complex<NumberRepr>(NumberRepr(0l), nb->GetValue());
    return ret_t(value, true);
  }
  // is it a + b*i?
  if (n->Type() == Node::Type_t::Summand) {
    auto summand = std::static_pointer_cast<Summand>(n);
    if (summand->Data().size() != 2) {
      return invalid;
    }
    auto it = summand->Data().begin();
    auto value1 = complex_number(*it);
    if (value1.second == false) {
      return invalid;
    }
    std::advance(it, 1);
    auto value2 = complex_number(*it);
    if (value2.second == false) {
      return invalid;
    }
    return ret_t(value1.first + value2.first, true);
  }

  return invalid;
}

NodePtr UserFunction::Eval(const std::list<NodePtr> &args, bool numeric) {

  NodePtr result;
  bool rep = SpecialValues(args, &result);
  if (rep) {
    return result;
  }

  std::vector<std::complex<NumberRepr>> dargs;
  dargs.reserve(args.size());

  bool num = true;
  bool real_values = false;
  for (const auto &a : args) {
    auto number = complex_number(a);
    if (number.second == false) {
      num = false;
      break;
    } else {
      dargs.push_back(number.first);
      if (!number.first.real().IsFraction() ||
          !number.first.imag().IsFraction()) {
        real_values = true;
      }
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

NodePtr FuncSin::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::sin(value));
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

NodePtr FuncCos::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::cos(value));
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

// --- tan --------------

NodePtr FuncTan::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::tan(value));
}

std::list<std::pair<NodePtr, std::string>> FuncTan::svalues = {
    {UserFunction::make_node("0"), "0"},
};

bool FuncTan::SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}

// --- asin --------------

NodePtr FuncASin::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::asin(value));
}

std::list<std::pair<NodePtr, std::string>> FuncASin::svalues = {
    {UserFunction::make_node("0"), "0"},
    {UserFunction::make_node("1"), "pi/2"},
    {UserFunction::make_node("-1"), "-pi/2"},
    {UserFunction::make_node("-3^(1/2)/2"), "-pi/3"},
    {UserFunction::make_node("-2^(1/2)/2"), "-pi/4"},
    {UserFunction::make_node("-1/2"), "-pi/6"},
    {UserFunction::make_node("3^(1/2)/2"), "pi/3"},
    {UserFunction::make_node("2^(1/2)/2"), "pi/4"},
    {UserFunction::make_node("1/2"), "pi/6"},
};

bool FuncASin::SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}

// --- acos --------------

NodePtr FuncACos::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::acos(value));
}

std::list<std::pair<NodePtr, std::string>> FuncACos::svalues = {
    {UserFunction::make_node("0"), "pi/2"},
    {UserFunction::make_node("1"), "0"},
    {UserFunction::make_node("-1"), "pi"},
    {UserFunction::make_node("-3^(1/2)/2"), "5/6*pi"},
    {UserFunction::make_node("-2^(1/2)/2"), "3/4*pi"},
    {UserFunction::make_node("-1/2"), "2/3*pi"},
    {UserFunction::make_node("3^(1/2)/2"), "pi/6"},
    {UserFunction::make_node("2^(1/2)/2"), "pi/4"},
    {UserFunction::make_node("1/2"), "pi/3"},
};

bool FuncACos::SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}

// --- atan --------------

NodePtr FuncATan::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::atan(value));
}

std::list<std::pair<NodePtr, std::string>> FuncATan::svalues = {
    {UserFunction::make_node("0"), "0"},
};

bool FuncATan::SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}

// --- exp --------------

NodePtr FuncExp::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::exp(value));
}

std::list<std::pair<NodePtr, std::string>> FuncExp::svalues = {
    {UserFunction::make_node("0"), "1"},
    {UserFunction::make_node("2*pi*i"), "1"},
    {UserFunction::make_node("pi*i"), "-1"},
    {UserFunction::make_node("1/2*pi*i"), "i"},
    {UserFunction::make_node("3/2*pi*i"), "-i"},
};

bool FuncExp::SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}

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
