//
//  Error.h
//  calculator
//
//  Created by Lennart Oymanns on 30.04.17.
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Error_h
#define Error_h

#include <exception>
#include <string>

namespace Equation {

class InputError : public std::exception {
public:
  explicit InputError(int position, const std::string &reason) {
    msg = reason;
    pos = position;
  }
  virtual const char *what() const throw() { return msg.c_str(); }
  int pos;

protected:
  std::string msg;
};

class InputNumberError : public InputError {
public:
  InputNumberError(int position, const std::string &reason)
      : InputError(position, reason) {}
};

class InputUnknownCharError : public InputError {
public:
  InputUnknownCharError(int position, const std::string &reason)
      : InputError(position, reason) {}
};

class InputEquationError : public InputError {
public:
  InputEquationError(int position, const std::string &reason)
      : InputError(position, reason) {}
};
}

#endif /* Error_h */
