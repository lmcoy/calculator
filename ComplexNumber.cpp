//
//  ComplexNumber.cpp
//  Project
//
//  Created by Lennart Oymanns on 14.07.17.
//
//

#include "ComplexNumber.hpp"

#include "Factor.hpp"
#include "Node.hpp"
#include "Number.hpp"
#include "Summand.hpp"
#include "Variable.hpp"

namespace Equation {

/** complex_number converts the node n to a complex number. If n does not
 * represent a complex number, the second argument of the return type is false.
 */
std::pair<std::complex<NumberRepr>, bool> complex_number(NodePtr n) {
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

/**
 node_from_complex generates a node pointer that represents the complex number
 c.
 */
NodePtr node_from_complex(std::complex<double> c) {
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

} // namespace Equation
