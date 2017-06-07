//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <string>
#include <vector>

#include "Node.hpp"
#include "Token.hpp"

namespace Equation {

class Parser {
public:
  NodePtr Parse(const std::string &expr);

private:
  NodePtr parseSummand();
  NodePtr parseFactor();
  NodePtr parseNumber();
  NodePtr parseFunction();
  NodePtr parsePower();

  const Token &readNextToken();
  void unreadToken();

  std::vector<Token> tokens;
  static const Token NoToken;
  int index = 0;
};
} // namespace Equation

#endif /* Parser_hpp */
