//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Lexer_hpp
#define Lexer_hpp

#include <vector>

#include "Token.hpp"

namespace Equation {

class Lexer {
public:
  Lexer(const std::string &expr) : expression(expr) {}

  std::vector<Token> TokenList();

  /**
   ReadToken returns the next token in the expression.
   */
  Token ReadToken();

private:
  /**
   peek returns the next character in the expression but the lexer does not
   advance to the next character, i.e. multiple calls of peek always return
   the same character.
   */
  char peek();

  /**
   read returns the next character in the expression and advances the lexer.
   */
  char read();

  /**
   unread reverts read()
   */
  void unread();

  /** readNumber reads a number from the expression. */
  std::string readNumber();

  /** readInteger reads a integer from the expression. */
  std::string readInteger();

  /** readRationalNumber reads a rational number from the expression.
   Note that the exponent notation is not supported. */
  std::string readRationalNumber();

  /** readString returns a string from the expression. */
  std::string readString();

  std::string expression;
  size_t index = 0;
};
} // namespace Equation

#endif /* Lexer_hpp */
