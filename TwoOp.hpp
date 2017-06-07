//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef TwoOp_hpp
#define TwoOp_hpp

#include <list>

#include "Node.hpp"
#include "NumberRepr.hpp"

namespace Equation {

class TwoOp : public Node {
public:
  TwoOp(NodePtr node, const std::string &operator1) {
    op1.push_back(node);
    o1 = operator1;
  }

  ~TwoOp() {}

  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false);

  void AddOp1(NodePtr node) { op1.push_back(node); }

  virtual NumberRepr Operation1(NumberRepr base, NumberRepr n) = 0;
  virtual NumberRepr NeutralElement() = 0;

  virtual void writeTreeToStream(std::ostream &s, const std::string &name);

  virtual bool IsNumber() const {
    bool number = true;
    for (auto &e : op1) {
      if (e->IsNumber() == false) {
        number = false;
      }
    }
    return number;
  }

  virtual bool equals(NodePtr n) const {
    if (Type() != n->Type()) {
      return false;
    }
    auto un = std::static_pointer_cast<TwoOp>(n);
    if (op1.size() != un->op1.size()) {
      return false;
    }
    bool num =
        std::is_permutation(op1.begin(), op1.end(), un->op1.begin(),
                            [](const NodePtr &item1, const NodePtr &item2) {
                              return (item1->equals(item2));
                            });
    // bool num = op1 == un->op1;
    if (!num) {
      return false;
    }
    return true;
  }

protected:
  std::list<NodePtr> op1;
  std::string o1;
};
} // namespace Equation

#endif /* TwoOp_hpp */
