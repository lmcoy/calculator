//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "Factor.hpp"
#include "Number.hpp"
#include "Power.hpp"
#include "Summand.hpp"

using namespace Equation;

namespace {

bool comparator(NodePtr &first, NodePtr &second) {
  if (first->Type() == Node::Type_t::Number) {
    if (second->Type() == Node::Type_t::Number) {
      auto f = std::static_pointer_cast<Number>(first)->GetValue();
      auto s = std::static_pointer_cast<Number>(second)->GetValue();
      if (f < s) {
        return true;
      }
      return false;
    }
    return true;
  }
  return false;
}
} // namespace

void Factor::ToStream(std::ostream &s) {

  int i = 0;
  for (auto &e : op1) {
    if (i != 0) {
      s << " * ";
    }
    bool brackets = false;
    if (e->Type() == Node::Type_t::Summand) {
      brackets = true;
    }
    if (brackets) {
      s << "(";
    }
    e->ToStream(s);
    if (brackets) {
      s << ")";
    }
    i += 1;
  }
}

void Factor::simplify(std::list<NodePtr> &op, std::shared_ptr<State> state) {
  for (auto it = op.begin(); it != op.end(); it++) {
    auto it2 = it;
    it2++;
    std::shared_ptr<Node> base;
    NodePtr exponent = std::make_shared<Summand>(std::make_shared<Number>(0l));
    if ((*it)->Type() == Node::Type_t::Power) {
      auto a = std::static_pointer_cast<Power>(*it);
      base = a->Base();
      auto e = std::static_pointer_cast<Summand>(exponent);
      e->AddOp1(a->Exponent());
    } else {
      base = *it;
      auto e = std::static_pointer_cast<Summand>(exponent);
      e->AddOp1(std::make_shared<Number>(1l));
    }
    for (; it2 != op.end();) {
      std::shared_ptr<Node> base2;
      std::shared_ptr<Node> exp2;
      if ((*it2)->Type() == Node::Type_t::Power) {
        auto a = std::static_pointer_cast<Power>(*it2);
        base2 = a->Base();
        exp2 = a->Exponent();
      } else {
        base2 = *it2;
        exp2 = std::make_shared<Number>(1l);
      }
      if (base->equals(base2)) {
        it2 = op.erase(it2);
        auto e = std::static_pointer_cast<Summand>(exponent);
        e->AddOp1(exp2);
      } else {
        it2++;
      }
    }

    exponent->Eval(&exponent, state);
    if (exponent->Type() == Node::Type_t::Number) {
      auto number = std::static_pointer_cast<Number>(exponent);
      if (number->GetValue() == NumberRepr(0l)) {
        it = op.erase(it);
        continue;
      }
      if (number->GetValue() == NumberRepr(1l)) {
        *it = base;
        continue;
      }
    }
    *it = std::make_shared<Power>(base, exponent);
  }
}

void Factor::Eval(NodePtr *base, std::shared_ptr<State> state, bool numeric) {
  TwoOp::Eval(base, state, numeric);
  // if a child is also a Factor, move elements to this Factor
  std::list<NodePtr> n;
  for (auto it = op1.begin(); it != op1.end(); it++) {
    if ((*it)->Type() == Node::Type_t::Factor) {
      auto fact = std::static_pointer_cast<Factor>(*it);
      n.insert(n.end(), fact->Data().begin(), fact->Data().end());
      it = op1.erase(it);
    }
  }
  op1.insert(op1.end(), n.begin(), n.end());

  if ((*base).get() != this) {
    return;
  }

  if (base && (*base)->Type() != Node::Type_t::Factor) {
    return;
  }

  simplify(op1, state);

  if (base && op1.size() == 1) {
    *base = op1.front();
    return;
  }

  if (base && op1.size() == 0) {
    base->reset(new Number(1l));
    return;
  }
  op1.sort(comparator);
}

static std::string latex_denom(NodePtr e) {
  std::stringstream s;
  if (e->Type() == Node::Type_t::Power) {
    auto p = std::static_pointer_cast<Power>(e);
    auto exp = p->Exponent();
    auto base = p->Base();
    if (exp->Type() == Node::Type_t::Number) {
      auto nb = std::static_pointer_cast<Number>(exp);
      if (nb->GetValue() < NumberRepr(0l)) {
        auto n = nb->GetValue();
        n *= NumberRepr(-1l);

        bool brackets = base->Type() == Node::Type_t::Summand ||
                        base->Type() == Node::Type_t::Factor ||
                        base->Type() == Node::Type_t::Power;

        if (brackets) {
          s << "\\left(";
        }
        base->ToLatex(s);
        if (brackets) {
          s << "\\right)";
        }
        if (n != NumberRepr(1l)) {
          s << "^{";
          s << n;
          s << "}";
        }
        return s.str();
      }
    }
  }
  return "";
}

void Factor::ToLatex(std::ostream &s) {
  bool frac = false;
  if (frac) {
    s << "\\frac{";
  }
  std::stringstream num;
  std::string denom = "";
  for (auto &e : op1) {

    auto d = latex_denom(e);
    if (d.size() > 0) {
      if (denom.size() != 0) {
        denom += " ";
      }
      denom += d;
      continue;
    }

    bool brackets = false;
    if (e->Type() == Node::Type_t::Summand) {
      brackets = true;
    }
    if (brackets) {
      num << "\\left(";
    }
    e->ToStream(num);
    if (brackets) {
      num << "\\right)";
    }
  }
  if (denom.size() != 0) {
    if (num.str().size() == 0) {
      s << "\\frac{1}{" << denom << "}";
      return;
    } else {
      s << "\\frac{" << num.str() << "}{" << denom << "}";
      return;
    }
  }
  s << num.str();
}
