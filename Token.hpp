//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Token_hpp
#define Token_hpp

#include <ostream>
#include <string>

namespace Equation {

class Token {
public:
  enum class Type_t {
    Operator,
    Number,
    String,
    Comma,
    End,
  } Type;

  Token(Token::Type_t type, const std::string &v, int position)
      : Type(type), Value(v), pos(position) {}

  std::string Value;
  int pos;
};
} // namespace Equation

std::ostream &operator<<(std::ostream &os, const Equation::Token::Type_t &obj);

#endif /* Token_hpp */
