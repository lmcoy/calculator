//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "Equation.hpp"
#include "Error.h"
#include "Parser.hpp"

namespace Equation {

bool Equation::Set(const std::string &eq, std::ostream &errorstream) {
  Parser parser;
  try {
    auto node = parser.Parse(eq);
    equation = node;
  } catch (InputError &e) {
    errorstream << "error: " << e.what() << "\n";
    errorstream << eq << "\n";
    for (int i = 0; i < e.pos; i++) {
      errorstream << '~';
    }
    errorstream << "^\n";
    return false;
  }

  return true;
}

std::string Equation::Evaluate(bool numeric, std::shared_ptr<State> state) {
  if (!equation) {
    return "";
  }
  equation->Eval(&equation, state, numeric);

  return equation->ToString();
}

void Equation::WriteInternalRepToStream(std::ostream &s) {
  if (!equation) {
    return;
  }
  equation->WriteTreeToStream(s);
}
bool Equation::operator==(const Equation &e) const {
  return equation->equals(e.equation);
}

std::string Equation::ToString() const { return equation->ToString(); }

std::string Equation::ToLatex() const {
  std::stringstream s;
  equation->ToLatex(s);
  return s.str();
}

} // namespace Equation
