//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <cmath>

#include "Factor.hpp"
#include "Number.hpp"
#include "Power.hpp"

using namespace Equation;

void Power::writeTreeToStream(std::ostream &s, const std::string &name) {
  std::string type = "op";
  s << name << "[label=<" << type << ", "
    << "^"
    << ">]\n";

  std::stringstream cbase;
  cbase << name << "pb";
  s << name << " -> " << cbase.str() << " [label=\""
    << "base"
    << "\"];\n";

  std::stringstream cexp;
  cexp << name << "pe";
  s << name << " -> " << cexp.str() << " [label=\""
    << "exp"
    << "\"];\n";

  base->writeTreeToStream(s, cbase.str());
  exponent->writeTreeToStream(s, cexp.str());
}

void Power::Eval(NodePtr *ba, std::shared_ptr<State> state, bool numeric) {
  base->Eval(&base, state, numeric);
  exponent->Eval(&exponent, state, numeric);
  // evaluate (x^s)^r
  if (base->Type() == Node::Type_t::Power &&
      exponent->Type() == Node::Type_t::Number) {
    auto b = std::static_pointer_cast<Power>(base);
    if (b->Exponent()->Type() == Node::Type_t::Number) {
      auto r = std::static_pointer_cast<Number>(b->Exponent());
      auto s = std::static_pointer_cast<Number>(exponent);
      auto nr = r->GetValue();
      auto ns = s->GetValue();
      if (nr.IsFraction() && ns.IsFraction()) {
        if (std::abs(nr.Denominator()) % 2 == 1 &&
            std::abs(ns.Denominator()) % 2 == 1) {
          base = b->Base();
          exponent = std::make_shared<Number>(nr * ns);
        }
      }
    }
  }
  // evaluate if base and exponent are numbers
  if (base->Type() == Node::Type_t::Number &&
      exponent->Type() == Node::Type_t::Number) {
    auto ptr1 = std::static_pointer_cast<Number>(base);
    auto ptr2 = std::static_pointer_cast<Number>(exponent);
    auto a = ptr1->GetValue();
    auto b = ptr2->GetValue();

    auto newnumber = NumberRepr::Pow(a, b);
    if (newnumber.IsValid()) {
      ba->reset(new Number(newnumber));
    }
  }
  // split (x*y)^n to x^n*y^n
  if (base->Type() == Node::Type_t::Factor) {
    auto factor = std::static_pointer_cast<Factor>(base);
    auto newfactor = std::make_shared<Factor>();
    for (auto &f : factor->Data()) {
      auto pow = std::make_shared<Power>(f, exponent->clone());
      auto parent = std::static_pointer_cast<Node>(pow);
      pow->Eval(&parent, state, numeric);
      newfactor->AddOp1(parent);
    }
    *ba = newfactor;
    return;
  }
}
