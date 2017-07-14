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
