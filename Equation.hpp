//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Equation_hpp
#define Equation_hpp

#include <iostream>
#include <memory>

#include "State.hpp"

namespace Equation {

class Node;

typedef std::shared_ptr<Node> NodePtr;

class Equation {
public:
  bool Set(const std::string &eq, std::ostream &errorstream = std::cerr);

  std::string
  Evaluate(bool numeric = false,
           std::shared_ptr<State> state = std::make_shared<DefaultState>());

  std::string ToString() const;

  std::string ToLatex() const;

  void WriteInternalRepToStream(std::ostream &s);

  bool operator==(const Equation &e) const;

private:
  NodePtr equation;
};
} // namespace Equation

#endif /* Equation_hpp */
