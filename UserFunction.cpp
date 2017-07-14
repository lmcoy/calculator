//
//  Created by Lennart Oymanns on 03.06.17.
//
#include <cmath>
#include <complex>

#include "ComplexNumber.hpp"
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
