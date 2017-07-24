//
//  Copyright © 2017 Lennart Oymanns. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#include "State.hpp"

namespace Equation {

class Node;

typedef std::shared_ptr<Node> NodePtr;

/** Node is a node of a tree that represents an equation.

 The class Node is an interface which is implemented by different parts of an
 equation such as Number, Summand, Factor, etc.
 */
class Node {
public:
  /** Type_t contains all allowed types for a node. */
  enum class Type_t {
    Number,     ///< Number
    Summand,    ///< sum, e.g. a + b + ...
    Factor,     ///< product, e.g. a * b * ...
    UnaryMinus, ///< leading minus, e.g. -a, -b, ...
    Variable,   ///< Variable
    Power,      ///< power, e.g. a^b, ...
    Function,   ///< function, e.g. f(x), g(x,y), ...
  };

  /** Eval evaluates the expression represented by this node.

   Evaluating can mean performing calculations like sums or products of numbers
   or collecting variables, e.g. x+x -> 2*x. The parameter numeric specifies if
   Eval performs numerical calculations, i.e. if expressions like sqrt(2) are
   evaluated to 1.41.... or not. The parameter state specifices all known
   variables and functions. */
  virtual void Eval(NodePtr *base, std::shared_ptr<State> state,
                    bool numeric = false) = 0;

  /** Type returns the type of this node. */
  virtual enum Type_t Type() const = 0;

  /** empty destructor */
  virtual ~Node() {}

  /** ToString returns a string representation of this node.
   see ToStream(). */
  virtual std::string ToString() {
    std::stringstream s;
    ToStream(s);
    return s.str();
  }

  /** ToLatex writes a latex representation of this node to the stream s. */
  virtual void ToLatex(std::ostream &s) = 0;

  /** IsNumber returns true if this node is a number after evaluation. */
  virtual bool IsNumber() const = 0;

  /** ToStream writes a string representation of this node to the stream s. */
  virtual void ToStream(std::ostream &s) = 0;

  /** WriteToStream writes this node as a graphviz digraph to stream s. */
  virtual void WriteTreeToStream(std::ostream &s) {
    s << "digraph g {\n";
    writeTreeToStream(s, "n");
    s << "}\n";
  }

  /** writeTreeToStream writes this node as a graphviz digraph to stream s.
   The parameter name is used to ensure unique names for all nodes. Any child of
   a node is expected to append a string to the name to make the name unique.
   Note that this function is supposed to be called by WriteTreeToStream which
   provides a basename and the digraph structure. */
  virtual void writeTreeToStream(std::ostream &s, const std::string &name) = 0;

  /** same as equals method. */
  bool operator==(const NodePtr n) const { return equals(n); }

  /** equals returns true if this node and Node n are equal (e.g. same type
   * etc.). */
  virtual bool equals(NodePtr n) const = 0;

  /** clone returns a shared pointer to a new Node with the same content as this
   * Node. */
  virtual NodePtr clone() const = 0;
};
} // namespace Equation

#endif /* Node_hpp */
