//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include <set>

#include "Variable.hpp"
#include "Number.hpp"

using namespace Equation;

void Variable::writeTreeToStream(std::ostream &s, const std::string &name) {
  std::string type = "v";
  s << name << "[label=<" << type << ", " << vname << ">]\n";
}

void Variable::ToLatex(std::ostream &s) {
  static std::set<std::string> vars = {
    "pi",    "lambda", "alpha", "beta",   "gamma",   "delta",   "epsilon",
    "zeta",  "eta",    "theta", "iota",   "kappa",   "mu",      "nu",
    "xi",    "rho",    "sigma", "tau",    "upsilon", "phi",     "chi",
    "psi",   "omega",  "Pi",    "Lambda", "Alpha",   "Beta",    "Gamma",
    "Delta", "Zeta",   "Theta", "Xi",     "Sigma",   "Upsilon", "Phi",
    "Psi",   "Omega"
  };
  auto isl = vars.find(vname);
  if (isl != vars.end()) {
    s << "\\";
  }
  s << vname;
}
