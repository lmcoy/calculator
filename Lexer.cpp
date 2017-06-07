//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <iostream>

#include "Error.h"
#include "Lexer.hpp"

using namespace Equation;

std::vector<Token> Lexer::TokenList() {
  std::vector<Token> tokens;
  while (1) {
    Token t = ReadToken();
    tokens.push_back(t);
    if (t.Type == Token::Type_t::End) {
      break;
    }
  }
  return tokens;
}

Token Lexer::ReadToken() {
  static const std::string operators = "()+-*/^";
  while (1) {
    char c = peek();
    if (c == 0) {
      return Token(Token::Type_t::End, "", index);
    }
    if (c == ' ') {
      read();
      continue;
    }
    if (isdigit(c) || c == '.') {
      return Token(Token::Type_t::Number, readNumber(), index);
    }
    if (isalpha(c)) {
      return Token(Token::Type_t::String, readString(), index);
    }
    if (c == ',') {
      read();
      return Token(Token::Type_t::Comma, std::string(1, c), index);
    }
    auto op = operators.find(c);
    if (op != std::string::npos) {
      read();
      return Token(Token::Type_t::Operator, std::string(1, c), index);
    }
    throw InputUnknownCharError(index, "unknown character");
  }
}

char Lexer::peek() {
  if (index >= expression.size()) {
    return 0;
  }
  return expression[index];
}

char Lexer::read() {
  char next = peek();
  if (next != 0) {
    index += 1;
  }
  return next;
}

void Lexer::unread() {
  if (index == 0) {
    return;
  }
  index -= 1;
}

std::string Lexer::readNumber() {
  std::string number = readRationalNumber();
  if (number == "") {
    return "";
  }
  char c = peek();
  if (c == 'e' || c == 'E') {
    read();
    std::string exponent = readInteger();
    if (exponent == "") {
      // error: no exponent
      throw InputNumberError(index, "no exponent");
    }
    number += c;
    number += exponent;
  }
  return number;
}

std::string Lexer::readRationalNumber() {
  std::string number = "";
  char c = peek();
  // leading + or -?
  if (c == '-' || c == '+') {
    read();
    number += c;
  }
  bool isnumber = false;
  std::string integer = readInteger();
  if (integer != "") {
    isnumber = true;
  }

  number += integer;

  c = peek();
  if (c == '.') {
    read();
    char c2 = peek();
    if (c2 == '+' || c2 == '-') {
      if (isnumber) {
        return number + c;
      }
      // error: only .
      throw InputNumberError(index, "not a number");
    }
    std::string fr = readInteger();
    if (!isnumber && fr == "") {
      // error: only .
      throw InputNumberError(index, "not a number");
    }
    if (fr != "" && (fr[0] == '+' || fr[0] == '-')) {
      // error: + or - after decimal point
      throw InputNumberError(index, "not a number");
    }
    number += c;
    number += fr;
  }
  return number;
}

std::string Lexer::readInteger() {
  std::string number = "";

  char c = peek();
  if (c == 0) {
    // error: no number
    return "";
  }
  // leading + or -?
  if (c == '-' || c == '+') {
    read();
    number += c;
  }
  // read number
  bool isnumber = false;
  while (1) {
    c = peek();
    if (isdigit(c)) {
      read();
      number += c;
      isnumber = true;
    } else {
      break;
    }
  }
  if (!isnumber) {
    return "";
  }

  return number;
}

std::string Lexer::readString() {
  std::string s = "";
  while (1) {
    char c = peek();
    if (isalnum(c) || c == '_') {
      read();
      s += c;
    } else {
      return s;
    }
  }
}
