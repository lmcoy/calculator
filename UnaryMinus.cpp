//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "UnaryMinus.hpp"
#include "Number.hpp"

using namespace Equation;

void UnaryMinus::writeTreeToStream(std::ostream &s, const std::string &name) {
  std::string type = "op";
  s << name << "[label=<" << type << ", "
    << "-"
    << ">]\n";
  s << name << " -> " << name + "u;\n";
  data->writeTreeToStream(s, name + "u");
}

void UnaryMinus::Eval(NodePtr *base, std::shared_ptr<State> state,
                      bool numeric) {
  data->Eval(&data, state, numeric);
  if (data->Type() == Node::Type::Number) {
    auto ptr = std::static_pointer_cast<Number>(data);
    auto value = ptr->GetValue();

    value *= NumberRepr(-1l);

    base->reset(new Number(value));
  }
}
