//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "Variable.hpp"
#include "Number.hpp"

using namespace Equation;

void Variable::writeTreeToStream(std::ostream &s, const std::string &name) {
  std::string type = "v";
  s << name << "[label=<" << type << ", " << vname << ">]\n";
}
