//
//  Created by Lennart Oymanns on 03.06.17.
//

#ifndef UserFunction_hpp
#define UserFunction_hpp

#include <list>

#include "NumberRepr.hpp"

namespace Equation {

class Node;
typedef std::shared_ptr<Node> NodePtr;

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

class UserFunction {
public:
  virtual NodePtr Eval(const std::list<NodePtr> &args, bool numeric = false);

  virtual NodePtr EvalNum(const std::vector<NumberRepr> &args) = 0;
  virtual bool SpecialValues(const std::list<NodePtr> &args,
                             NodePtr *result) = 0;
  virtual size_t NumArgs() const = 0;

  static NodePtr make_node(const std::string &expr);
};

class FuncSin : public UserFunction {
public:
  virtual NodePtr EvalNum(const std::vector<NumberRepr> &args);
  virtual bool SpecialValues(const std::list<NodePtr> &args, NodePtr *result);
  virtual size_t NumArgs() const { return 1; }

  static std::list<std::pair<NodePtr, std::string>> svalues;
};

class FuncCos : public UserFunction {
public:
  virtual NodePtr EvalNum(const std::vector<NumberRepr> &args);
  virtual bool SpecialValues(const std::list<NodePtr> &args, NodePtr *result);
  virtual size_t NumArgs() const { return 1; }

  static std::list<std::pair<NodePtr, std::string>> svalues;
};

class Derivative : public UserFunction {
public:
  Derivative();
  virtual size_t NumArgs() const { return 2; }
  virtual NodePtr Eval(const std::list<NodePtr> &args, bool numeric = false);

  virtual NodePtr EvalNum(const std::vector<NumberRepr> &args) { return 0; }
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

#endif /* UserFunction_hpp */
