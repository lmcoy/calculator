//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//
#include <iostream>
#include <map>

#include "Error.h"
#include "Factor.hpp"
#include "Function.hpp"
#include "Lexer.hpp"
#include "Number.hpp"
#include "Parser.hpp"
#include "Power.hpp"
#include "Summand.hpp"
#include "UnaryMinus.hpp"
#include "Variable.hpp"

using namespace Equation;

const Token Parser::NoToken(Token::Type_t::End, "NoToken", 0);

NodePtr Parser::Parse(const std::string &expr) {
  Lexer lexer(expr);
  tokens = lexer.TokenList();
  index = 0;

  auto nodes = parseSummand();
  auto token = readNextToken();
  if (token.Type != Token::Type_t::End) {
    if (token.Type == Token::Type_t::Operator && token.Value == ")") {
      throw InputEquationError(token.pos - 1, "unmatched ')'");
    }
    if (token.Type == Token::Type_t::Operator && token.Value == "^") {
      throw InputEquationError(token.pos - 1, "multiple '^' not supported");
    }
    throw InputEquationError(token.pos - 1, "expected end of expression");
  }
  return nodes;
}

NodePtr Parser::parseSummand() {
  auto first = parseFactor();
  auto summand = std::make_shared<Summand>(first);

  bool valid = false;
  auto token = readNextToken();
  while (token.Type == Token::Type_t::Operator &&
         (token.Value == "+" || token.Value == "-")) {
    auto next = parseFactor();
    if (token.Value == "+") {
      summand->AddOp1(next);
    } else {
      auto m = std::make_shared<UnaryMinus>(next);
      summand->AddOp1(m);
    }
    valid = true;
    token = readNextToken();
  }
  unreadToken();

  if (valid) {
    return summand;
  }
  return first;
}

NodePtr Parser::parseFactor() {
  auto first = parsePower();
  auto factor = std::make_shared<Factor>(first);

  bool valid = false;
  auto token = readNextToken();
  while (token.Type == Token::Type_t::Operator &&
         (token.Value == "*" || token.Value == "/")) {
    auto next = parsePower();
    if (token.Value == "*") {
      factor->AddOp1(next);
    } else {
      auto d = std::make_shared<Power>(next, std::make_shared<Number>(-1l));
      factor->AddOp1(d);
      // factor->AddOp2(next);
    }
    valid = true;
    token = readNextToken();
  }
  unreadToken();

  if (valid) {
    return factor;
  }
  return first;
}

NodePtr Parser::parsePower() {
  auto base = parseNumber();

  auto token = readNextToken();
  if (token.Type == Token::Type_t::Operator && token.Value == "^") {
    auto exponent = parseNumber();
    auto power = std::make_shared<Power>(base, exponent);
    return power;
  }
  unreadToken();

  return base;
}

NodePtr Parser::parseNumber() {
  const Token &token = readNextToken();

  if (token.Type == Token::Type_t::Operator && token.Value == "(") {
    auto v = parseSummand();
    auto t = readNextToken();
    if (t.Type != Token::Type_t::Operator || token.Value == ")") {
      throw InputEquationError(t.pos, "missing ')'");
    }
    return v;
  }
  if (token.Type == Token::Type_t::Operator && token.Value == "-") {
    return std::make_shared<UnaryMinus>(parseNumber());
  }

  if (token.Type == Token::Type_t::String) {
    const Token &next = readNextToken();
    unreadToken();
    if (next.Type == Token::Type_t::Operator && next.Value == "(") {
      // function
      unreadToken(); // unread function name
      return parseFunction();
    }
    return std::make_shared<Variable>(token.Value);
  }

  if (token.Type == Token::Type_t::Number) {
    return std::make_shared<Number>(Number(token.Value));
  }

  if (token.Type == Token::Type_t::End) {
    throw InputEquationError(token.pos, "unexpected end of expression");
  }

  throw InputEquationError(token.pos, "unknown token type");
  return 0;
}

NodePtr Parser::parseFunction() {
  auto name = readNextToken();
  if (name.Type != Token::Type_t::String) {
    throw InputEquationError(name.pos, "not a valid function name");
  }
  auto open = readNextToken();
  if (open.Type != Token::Type_t::Operator || open.Value != "(") {
    throw InputEquationError(open.pos, "missing '(' after function");
  }

  auto first = parseSummand();
  auto fun = std::make_shared<Function>(name.Value);
  fun->AddArg(first);

  auto token = readNextToken();
  while (token.Type == Token::Type_t::Comma && token.Value == ",") {
    auto next = parseSummand();
    fun->AddArg(next);
    token = readNextToken();
  }
  if (token.Type != Token::Type_t::Operator || token.Value != ")") {
    throw InputEquationError(token.pos, "missing ')'");
  }

  return fun;
}

const Token &Parser::readNextToken() {
  if (index >= tokens.size()) {
    return Parser::NoToken;
  }
  index += 1;
  return tokens[index - 1];
}

void Parser::unreadToken() {
  if (index == 0) {
    return;
  }
  index -= 1;
}
