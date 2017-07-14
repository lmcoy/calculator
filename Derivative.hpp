//
//  Created by Lennart Oymanns on 14.07.17.
//

#ifndef Derivative_hpp
#define Derivative_hpp

#include <map>
#include <string>
#include <vector>

#include "UserFunction.hpp"

namespace Equation {
class Number;
typedef std::shared_ptr<Number> NumberPtr;

class Variable;
typedef std::shared_ptr<Variable> VariablePtr;

class Summand;
typedef std::shared_ptr<Summand> SummandPtr;

class Power;
typedef std::shared_ptr<Power> PowerPtr;

class UnaryMinus;
typedef std::shared_ptr<UnaryMinus> UnaryMinusPtr;

class Factor;
typedef std::shared_ptr<Factor> FactorPtr;

class Function;
typedef std::shared_ptr<Function> FunctionPtr;

class Derivative : public UserFunction {
public:
  Derivative();
  virtual size_t NumArgs() const { return 2; }
  virtual NodePtr Eval(const std::list<NodePtr> &args, bool numeric = false);

  virtual NodePtr EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
    return 0;
  }
  virtual bool SpecialValues(const std::list<NodePtr> &args, NodePtr *result) {
    return false;
  }

private:
  NodePtr Dnumber(const NumberPtr &nb);
  NodePtr Dvariable(const VariablePtr &v, const std::string &var);
  NodePtr Dsum(const SummandPtr &v, const std::string &var);
  NodePtr Dpower(const PowerPtr &v, const std::string &var);
  NodePtr Dminus(const UnaryMinusPtr &v, const std::string &var);
  NodePtr Dmul(const FactorPtr &v, const std::string &var);
  NodePtr Dfunc(const FunctionPtr &v, const std::string &var);

  std::map<std::string, NodePtr> functions;
};

} // namespace Equation

#endif
