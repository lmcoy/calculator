//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include <algorithm>

#include "ComplexNumber.hpp"
#include "Factor.hpp"
#include "Number.hpp"
#include "Summand.hpp"
#include "UnaryMinus.hpp"

using namespace Equation;

static bool comparator_sum(NodePtr &first, NodePtr &second) {
  auto cplx1 = complex_number(first);
  if (!cplx1.second) {
    return false;
  }
  auto cplx2 = complex_number(second);
  if (!cplx2.second) {
    return false;
  }
  if (cplx1.first.imag() == NumberRepr(0l) &&
      cplx2.first.imag() != NumberRepr(0l)) {
    return true;
  }
  return false;
}

void Summand::ToStream(std::ostream &s) {
  int i = 0;
  for (auto &e : op1) {
    if (i != 0) {
      if (e->Type() == Node::Type_t::UnaryMinus) {
        s << " - ";
        auto un = std::static_pointer_cast<UnaryMinus>(e);
        un->ToStreamAbs(s);
      } else if (e->Type() == Node::Type_t::Factor) {
        auto factor = std::static_pointer_cast<Factor>(e);
        auto pre = factor->Data().begin();
        if ((*pre)->Type() == Node::Type_t::Number) {
          auto nb = std::static_pointer_cast<Number>(*pre);
          if (nb->GetValue() < NumberRepr(0l)) {
            s << " ";
            e->ToStream(s);
          } else {
            s << " + ";
            e->ToStream(s);
          }
        } else {
          s << " + ";
          e->ToStream(s);
        }
      } else if (e->Type() == Node::Type_t::Number) {
        auto nb = std::static_pointer_cast<Number>(e);
        auto r = nb->GetValue();
        if (nb->GetValue() < NumberRepr(0l)) {
          s << " - ";
          r *= NumberRepr(-1l);
        } else {
          s << " + ";
        }
        s << r.String();
      } else {
        s << " + ";
        e->ToStream(s);
      }
    } else {
      e->ToStream(s);
    }
    i += 1;
  }
}

void Summand::ToLatex(std::ostream &s) {
  int i = 0;
  for (auto &e : op1) {
    if (i != 0) {
      if (e->Type() == Node::Type_t::UnaryMinus) {
        s << " - ";
        auto un = std::static_pointer_cast<UnaryMinus>(e);
        un->ToStreamAbs(s);
      } else if (e->Type() == Node::Type_t::Factor) {
        auto factor = std::static_pointer_cast<Factor>(e);
        auto pre = factor->Data().begin();
        if ((*pre)->Type() == Node::Type_t::Number) {
          auto nb = std::static_pointer_cast<Number>(*pre);
          if (nb->GetValue() < NumberRepr(0l)) {
            s << " ";
            e->ToLatex(s);
          } else {
            s << " + ";
            e->ToStream(s);
          }
        }
      } else {
        s << " + ";
        e->ToLatex(s);
      }
    } else {
      e->ToLatex(s);
    }
    i += 1;
  }
}

std::pair<NumberRepr, NodePtr> clone_expr_wo_coeff(NodePtr n) {
  if (n->Type() == Node::Type_t::Factor) {

    auto factor = std::static_pointer_cast<Factor>(n);
    auto it_first = factor->Data().begin();

    std::shared_ptr<Factor> factor_new;
    NodePtr result;
    auto type = (*it_first)->Type();
    if (type == Node::Type_t::Number) {
      factor_new = std::static_pointer_cast<Factor>(factor->clone());
      factor_new->Data().pop_front();
      if (factor_new->Data().size() == 1) {
        result = factor_new->Data().front();
      } else if (factor_new->Data().size() == 0) {
        return std::pair<NumberRepr, NodePtr>(NumberRepr(1l),
                                              (*it_first)->clone());
      } else {
        result = factor_new;
      }
      const auto coeff = std::static_pointer_cast<Number>(*it_first);
      return std::pair<NumberRepr, NodePtr>(coeff->GetValue(), result);
    }
    return std::pair<NumberRepr, NodePtr>(NumberRepr(1l), n->clone());
  }
  if (n->Type() == Node::Type_t::UnaryMinus) {
    auto un = std::static_pointer_cast<UnaryMinus>(n);
    auto res = clone_expr_wo_coeff(un->Data());
    res.first *= NumberRepr(-1l);
    return res;
  }

  return std::pair<NumberRepr, NodePtr>(NumberRepr(1l), n->clone());
}

void Summand::simplify() {
  std::list<std::pair<NumberRepr, NodePtr>> polynom;
  for (auto &e : op1) {
    auto nom = clone_expr_wo_coeff(e);
    auto pos = std::find_if(polynom.begin(), polynom.end(),
                            [&nom](const std::pair<NumberRepr, NodePtr> &n) {
                              return n.second->equals(nom.second);
                            });
    if (pos == polynom.end()) {
      polynom.push_back(nom);
    } else {
      pos->first += nom.first;
    }
  }
  // fill new summands
  op1.clear();
  for (auto &e : polynom) {
    if (e.first == NumberRepr(1l)) {
      op1.push_back(e.second);
    } else if (e.first == NumberRepr(0l)) {
      continue;
    } else {
      auto coeff = std::make_shared<Number>(e.first);
      std::shared_ptr<Factor> factor;
      if (e.second->Type() == Node::Type_t::Factor) {
        factor = std::static_pointer_cast<Factor>(e.second);
        factor->Data().push_front(coeff);
      } else {
        factor = std::make_shared<Factor>(coeff);
        factor->AddOp1(e.second);
      }
      op1.push_back(factor);
    }
  }
}

void Summand::Eval(NodePtr *base, std::shared_ptr<State> state, bool numeric) {
  TwoOp::Eval(base, state, numeric);

  if ((*base).get() != this) {
    return;
  }

  if (base && (*base)->Type() != Node::Type_t::Summand) {
    return;
  }

  simplify();

  if (base && op1.size() == 0) {
    base->reset(new Number(0l));
    return;
  }

  if (base && op1.size() == 1) {
    *base = op1.front();
    return;
  }
  op1.sort(comparator_sum);
}
