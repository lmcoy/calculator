//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <cmath>

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
  if (base->Type() == Node::Type::Power &&
      exponent->Type() == Node::Type::Number) {
    auto b = std::static_pointer_cast<Power>(base);
    if (b->Exponent()->Type() == Node::Type::Number) {
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
  if (base->Type() == Node::Type::Number &&
      exponent->Type() == Node::Type::Number) {
    auto ptr1 = std::static_pointer_cast<Number>(base);
    auto ptr2 = std::static_pointer_cast<Number>(exponent);
    auto a = ptr1->GetValue();
    auto b = ptr2->GetValue();

    auto newnumber = NumberRepr::Pow(a, b);
    if (newnumber.IsValid()) {
      ba->reset(new Number(newnumber));
    }
  }
}
