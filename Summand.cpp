//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "Summand.hpp"

using namespace Equation;

void Summand::ToStream(std::ostream &s) {
  int i = 0;
  for (auto &e : op1) {
    if (i != 0) {
      s << " + ";
    }
    e->ToStream(s);
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
