//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "TwoOp.hpp"
#include "Number.hpp"

using namespace Equation;

void TwoOp::Eval(NodePtr *base, std::shared_ptr<State> state, bool numeric) {
  for (auto &e : op1) {
    e->Eval(&e, state, numeric);
  }

  NumberRepr value = NeutralElement();
  for (auto it = op1.begin(); it != op1.end();) {
    if ((*it)->Type() == Node::Type::Number) {
      auto ptr = std::static_pointer_cast<Number>(*it);
      value = Operation1(value, ptr->GetValue());
      it = op1.erase(it);
    } else {
      it++;
    }
  }

  if (base && op1.size() == 0) {
    base->reset(new Number(value));
    return;
  }
  if (value != NeutralElement()) {
    auto newnumber = std::make_shared<Number>(value);
    op1.push_back(newnumber);
  }
}

void TwoOp::writeTreeToStream(std::ostream &s, const std::string &name) {
  std::string type = "op";
  std::stringstream ss;
  ss << NeutralElement();
  s << name << "[label=<" << type << ", " << ss.str() << ">]\n";
  for (int i = 0; i < op1.size(); i++) {
    std::stringstream cname;
    cname << name << "l" << i;
    s << name << " -> " << cname.str() << " [label=\"" << o1 << "\"];\n";
  }
  int i = 0;
  for (auto it = op1.begin(); it != op1.end(); it++, i++) {
    std::stringstream cname;
    cname << name << "l" << i;
    (*it)->writeTreeToStream(s, cname.str());
  }
}
