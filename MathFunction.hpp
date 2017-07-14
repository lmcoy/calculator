//
//  Created by Lennart Oymanns on 14.07.17.
//

#ifndef MathFunction_hpp
#define MathFunction_hpp

#include <list>
#include <vector>

#include "UserFunction.hpp"

namespace Equation {

class MathFunction : public UserFunction {
public:
  virtual bool SpecialValues(const std::list<NodePtr> &args, NodePtr *result);
  virtual size_t NumArgs() const { return 1; }

  void AddSpecialValue(NodePtr n, const std::string &s);

private:
  std::list<std::pair<NodePtr, std::string>> svalues;
};

class FuncSin : public MathFunction {
public:
  FuncSin();
  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args);
};

class FuncCos : public MathFunction {
public:
  FuncCos();
  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args);
};

class FuncTan : public MathFunction {
public:
  FuncTan();
  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args);
};

class FuncASin : public MathFunction {
public:
  FuncASin();
  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args);
};

class FuncACos : public MathFunction {
public:
  FuncACos();
  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args);
};

class FuncATan : public MathFunction {
public:
  FuncATan();
  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args);
};

class FuncExp : public MathFunction {
public:
  FuncExp();
  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args);
};

} // namespace Equation

#endif
