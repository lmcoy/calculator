//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#include "Token.hpp"

using namespace Equation;

std::ostream &operator<<(std::ostream &os, const Token::Type_t &obj) {
  switch (obj) {
  case Token::Type_t::Operator:
    os << std::string("Op");
    break;
  case Token::Type_t::Number:
    os << std::string("Nb");
    break;
  case Token::Type_t::String:
    os << std::string("St");
    break;
  case Token::Type_t::End:
    os << std::string("En");
    break;
  case Token::Type_t::Comma:
    os << std::string("Co");
    break;
  }
  return os;
}
