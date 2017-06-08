//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "Summand.hpp"
#include "UnaryMinus.hpp"

using namespace Equation;

void Summand::ToStream(std::ostream &s) {
  int i = 0;
  for (auto &e : op1) {
    if (i != 0) {
      if (e->Type() != Node::Type_t::UnaryMinus) {
        s << " + ";
        e->ToStream(s);
      } else {
        s << " - ";
        auto un = std::static_pointer_cast<UnaryMinus>(e);
        un->ToStreamAbs(s);
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
      s << " + ";
    }
    e->ToLatex(s);
    i += 1;
  }
}
