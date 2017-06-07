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

class UserFunction {
public:
  virtual NodePtr Eval(const std::list<NodePtr> &args, bool numeric = false);

  virtual NodePtr EvalNum(const std::vector<NumberRepr> &args) = 0;
  virtual bool SpecialValues(const std::list<NodePtr> &args,
                             NodePtr *result) = 0;
  virtual int NumArgs() const = 0;

  static NodePtr make_node(const std::string &expr);
};

class FuncSin : public UserFunction {
public:
  virtual NodePtr EvalNum(const std::vector<NumberRepr> &args);
  virtual bool SpecialValues(const std::list<NodePtr> &args, NodePtr *result);
  virtual int NumArgs() const { return 1; }

  static std::list<std::pair<NodePtr, std::string>> svalues;
};

class FuncCos : public UserFunction {
public:
  virtual NodePtr EvalNum(const std::vector<NumberRepr> &args);
  virtual bool SpecialValues(const std::list<NodePtr> &args, NodePtr *result);
  virtual int NumArgs() const { return 1; }

  static std::list<std::pair<NodePtr, std::string>> svalues;
};

} // namespace Equation

#endif /* UserFunction_hpp */
