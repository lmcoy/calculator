//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "Number.hpp"

using namespace Equation;

void Number::writeTreeToStream(std::ostream &s, const std::string &name) {
  std::string type = "nb";
  s << name << "[label=<" << type << ", " << value << ">]\n";
}
