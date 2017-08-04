//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <cmath>

#include "Factor.hpp"
#include "Number.hpp"
#include "Power.hpp"
#include "Variable.hpp"

using namespace Equation;

namespace {

Integer_t power(const Integer_t &base, int n) {
  Integer_t result(1l);
  for (int i = 0; i < n; i++) {
    result *= base;
  }
  return result;
}

// returns (factor, root) such that A = factor * (root)^(1/n)
std::pair<Integer_t, Integer_t> root_n(Integer_t A, int n) {
  Integer_t a(2);
  int num = 0;
  Integer_t factor(1l);
  Integer_t root(1l);
  while (A > 1) {
    if (A % a == 0) {
      num += 1;
      A /= a;
    } else {
      if (num > 0) {
        factor *= power(a, num / n);
        root *= power(a, num % n);
      }
      a += 1;
      num = 0;
    }
  }
  if (num > 0) {
    factor *= power(a, num / n);
    root *= power(a, num % n);
  }
  root *= A;

  return std::pair<Integer_t, Integer_t>(factor, root);
}

} // namespace

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
        if (boost::multiprecision::abs(nr.Denominator()) % Integer_t(2) ==
                Integer_t(1) &&
            boost::multiprecision::abs(ns.Denominator()) % Integer_t(2) ==
                Integer_t(1)) {
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
      return;
    }
  }
  // imaginary i
  if (base->Type() == Node::Type_t::Variable &&
      exponent->Type() == Node::Type_t::Number) {
    auto var = std::static_pointer_cast<Variable>(base);
    auto e = std::static_pointer_cast<Number>(exponent);
    auto ex = e->GetValue();
    if (var->Name() == "i" && ex.IsFraction() && ex.Denominator() == 1l) {
      auto numabs = boost::multiprecision::abs(ex.Numerator());
      NodePtr imag;
      if (ex.Numerator() > Integer_t(0)) {
        imag = std::make_shared<Variable>("i");
      } else {
        imag = std::make_shared<Power>(std::make_shared<Variable>("i"),
                                       std::make_shared<Number>(-1l));
      }

      Integer_t tmp = (numabs - Integer_t(1)) % Integer_t(4);
      if (tmp == Integer_t(0)) {
        *ba = imag;
        return;
      }
      if (tmp == Integer_t(2)) {
        auto newfactor = std::make_shared<Factor>();
        newfactor->AddOp1(std::make_shared<Number>(-1l));
        newfactor->AddOp1(imag);
        *ba = newfactor;
        return;
      }

      if (tmp == Integer_t(1)) {
        *ba = std::make_shared<Number>(-1l);
        return;
      }
      if (tmp == Integer_t(3)) {
        *ba = std::make_shared<Number>(1l);
        return;
      }
    }
  }
  // evalutate roots
  if (exponent->Type() == Node::Type_t::Number &&
      base->Type() == Node::Type_t::Number) {
    auto e = std::static_pointer_cast<Number>(exponent);
    auto b = std::static_pointer_cast<Number>(base);
    if (b->GetValue() == NumberRepr(1l)) {
      *ba = std::make_shared<Number>(1l);
      return;
    }
    if (e->GetValue().IsFraction() && b->GetValue().IsFraction()) {
      auto denom = e->GetValue().Denominator();
      if (denom > Integer_t(1) && denom < Integer_t(10)) {
        int d = denom.convert_to<int>();
        bool add_imag = false;
        bool add_neg = false;

        auto b_num = b->GetValue().Numerator();
        auto b_denom = b->GetValue().Denominator();

        // can we use i?
        if (b->GetValue() < NumberRepr(0l) &&
            e->GetValue() == NumberRepr(Rational_t(1, 2))) {
          add_imag = true;
          b_num = boost::multiprecision::abs(b_num);
          b_denom = boost::multiprecision::abs(b_denom);
        }
        if (b->GetValue() < NumberRepr(0l) &&
            e->GetValue() == NumberRepr(Rational_t(-1, 2))) {
          add_neg = true;
          add_imag = true;
          b_num = boost::multiprecision::abs(b_num);
          b_denom = boost::multiprecision::abs(b_denom);
        }
        auto base_num = root_n(b_num, d);
        auto base_denom = root_n(b_denom, d);

        auto factor = std::make_shared<Number>(
            Rational_t(base_num.first, base_denom.first));
        auto root = std::make_shared<Number>(
            Rational_t(base_num.second, base_denom.second));
        if (base_num.first == base_denom.first && !add_imag) {
          // can not calculate root of base
          return;
        }

        auto newpower_exp = std::make_shared<Number>(e->GetValue().Numerator());
        auto newfactor = std::make_shared<Factor>();
        newfactor->AddOp1(std::make_shared<Power>(factor, newpower_exp));
        newfactor->AddOp1(std::make_shared<Power>(root, exponent->clone()));
        if (add_imag) {
          newfactor->AddOp1(std::make_shared<Variable>("i"));
        }
        if (add_neg) {
          newfactor->AddOp1(std::make_shared<Number>(-1l));
        }
        auto eval = std::static_pointer_cast<Node>(newfactor);
        eval->Eval(&eval, state, numeric);
        *ba = eval;
        return;
      }
    }
  }
  // split (x*y)^n to x^n*y^n
  if (base->Type() == Node::Type_t::Factor) {
    auto factor = std::static_pointer_cast<Factor>(base);
    auto newfactor = std::make_shared<Factor>();
    for (auto &f : factor->Data()) {
      auto pow = std::make_shared<Power>(f, exponent->clone());
      auto parent = std::static_pointer_cast<Node>(pow);
      parent->Eval(&parent, state, numeric);
      newfactor->AddOp1(parent);
    }
    auto eval = std::static_pointer_cast<Node>(newfactor);
    eval->Eval(&eval, state, numeric);
    *ba = eval;
    return;
  }
}

void Power::ToStream(std::ostream &s) {
  bool brackets_b = false;
  if (base->Type() == Node::Type_t::Factor ||
      base->Type() == Node::Type_t::Power ||
      base->Type() == Node::Type_t::UnaryMinus ||
      base->Type() == Node::Type_t::Summand) {
    brackets_b = true;
  }
  bool brackets_e = false;
  if (exponent->Type() == Node::Type_t::Factor ||
      exponent->Type() == Node::Type_t::Power ||
      exponent->Type() == Node::Type_t::UnaryMinus ||
      exponent->Type() == Node::Type_t::Summand) {
    brackets_e = true;
  }
  if (base->Type() == Node::Type_t::Number) {
    auto nb = std::static_pointer_cast<Number>(base);
    auto r = nb->GetValue();
    if (r.IsFraction() && r.Denominator() != 1l) {
      brackets_b = true;
    }
    if (r < NumberRepr(0l)) {
      brackets_b = true;
    }
  }
  if (exponent->Type() == Node::Type_t::Number) {
    auto nb = std::static_pointer_cast<Number>(exponent);
    auto r = nb->GetValue();
    if (r.IsFraction() && r.Denominator() != 1l) {
      brackets_e = true;
    }
    if (r < NumberRepr(0l)) {
      brackets_e = true;
    }
  }

  if (brackets_b) {
    s << "(";
  }
  base->ToStream(s);
  if (brackets_b) {
    s << ")";
  }
  s << " ^ ";
  if (brackets_e) {
    s << "(";
  }
  exponent->ToStream(s);
  if (brackets_e) {
    s << ")";
  }
}

void Power::ToLatex(std::ostream &s) {
  bool brackets_b = false;
  if (base->Type() == Node::Type_t::Factor ||
      base->Type() == Node::Type_t::Power ||
      base->Type() == Node::Type_t::Summand) {
    brackets_b = true;
  }
  bool brackets_e = true;

  if (exponent->Type() == Node::Type_t::Number) {
    auto number = std::static_pointer_cast<Number>(exponent);
    if (number->GetValue() > NumberRepr(0l)) {
      // don't use brackets for positive numbers
      brackets_e = false;
    }
    if (number->GetValue() == NumberRepr(Rational_t(1l, 2l))) {
      // print as sqrt
      s << "\\sqrt{";
      base->ToLatex(s);
      s << "}";
      return;
    }
  }
  if (exponent->Type() == Node::Type_t::Variable) {
    // single variable does not need {} in exponent
    brackets_e = false;
  }

  if (brackets_b) {
    s << "\\left(";
  }
  base->ToLatex(s);
  if (brackets_b) {
    s << "\\right)";
  }
  s << "^";
  if (brackets_e) {
    s << "{";
  }
  exponent->ToLatex(s);
  if (brackets_e) {
    s << "}";
  }
}

bool Power::equals(NodePtr n) const {
  if (n->Type() != Node::Type_t::Power) {
    return false;
  }
  auto un = std::static_pointer_cast<Power>(n);
  return base->equals(un->base) && exponent->equals(un->exponent);
}
