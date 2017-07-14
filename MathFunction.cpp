#include "MathFunction.hpp"
#include "ComplexNumber.hpp"
#include "Node.hpp"

namespace Equation {

bool MathFunction::SpecialValues(const std::list<NodePtr> &args,
                                 NodePtr *result) {
  NodePtr arg0 = *(args.begin());
  for (auto &s : svalues) {
    if (s.first->equals(arg0)) {
      *result = make_node(s.second);
      return true;
    }
  }
  return false;
}

void MathFunction::AddSpecialValue(NodePtr n, const std::string &s) {
  svalues.push_back(std::pair<NodePtr, std::string>(n, s));
}

// -- sin ---

FuncSin::FuncSin() {
  AddSpecialValue(UserFunction::make_node("0"), "0");
  AddSpecialValue(UserFunction::make_node("pi"), "0");
  AddSpecialValue(UserFunction::make_node("2*pi"), "0");
  AddSpecialValue(UserFunction::make_node("1/2*pi"), "1");
  AddSpecialValue(UserFunction::make_node("3*pi/2"), "-1");
}

NodePtr FuncSin::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::sin(value));
}

// --- cos --------------

FuncCos::FuncCos() {
  AddSpecialValue(UserFunction::make_node("0"), "1");
  AddSpecialValue(UserFunction::make_node("pi"), "-1");
  AddSpecialValue(UserFunction::make_node("2*pi"), "1");
  AddSpecialValue(UserFunction::make_node("1/2*pi"), "0");
  AddSpecialValue(UserFunction::make_node("3*pi/2"), "0");
}

NodePtr FuncCos::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::cos(value));
}

// --- tan --------------

NodePtr FuncTan::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::tan(value));
}

FuncTan::FuncTan() { AddSpecialValue(UserFunction::make_node("0"), "0"); }

// --- asin --------------

NodePtr FuncASin::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::asin(value));
}

FuncASin::FuncASin() {
  AddSpecialValue(UserFunction::make_node("0"), "0");
  AddSpecialValue(UserFunction::make_node("1"), "pi/2");
  AddSpecialValue(UserFunction::make_node("-1"), "-pi/2");
  AddSpecialValue(UserFunction::make_node("-3^(1/2)/2"), "-pi/3");
  AddSpecialValue(UserFunction::make_node("-2^(1/2)/2"), "-pi/4");
  AddSpecialValue(UserFunction::make_node("-1/2"), "-pi/6");
  AddSpecialValue(UserFunction::make_node("3^(1/2)/2"), "pi/3");
  AddSpecialValue(UserFunction::make_node("2^(1/2)/2"), "pi/4");
  AddSpecialValue(UserFunction::make_node("1/2"), "pi/6");
}

// --- acos --------------

NodePtr FuncACos::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::acos(value));
}

FuncACos::FuncACos() {
  AddSpecialValue(UserFunction::make_node("0"), "pi/2");
  AddSpecialValue(UserFunction::make_node("1"), "0");
  AddSpecialValue(UserFunction::make_node("-1"), "pi");
  AddSpecialValue(UserFunction::make_node("-3^(1/2)/2"), "5/6*pi");
  AddSpecialValue(UserFunction::make_node("-2^(1/2)/2"), "3/4*pi");
  AddSpecialValue(UserFunction::make_node("-1/2"), "2/3*pi");
  AddSpecialValue(UserFunction::make_node("3^(1/2)/2"), "pi/6");
  AddSpecialValue(UserFunction::make_node("2^(1/2)/2"), "pi/4");
  AddSpecialValue(UserFunction::make_node("1/2"), "pi/3");
}

// --- atan --------------

NodePtr FuncATan::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::atan(value));
}

FuncATan::FuncATan() { AddSpecialValue(UserFunction::make_node("0"), "0"); }

// --- exp --------------

NodePtr FuncExp::EvalNum(const std::vector<std::complex<NumberRepr>> &args) {
  auto tmp = args[0];
  std::complex<double> value(tmp.real().Double(), tmp.imag().Double());
  return node_from_complex(std::exp(value));
}

FuncExp::FuncExp() {
  AddSpecialValue(UserFunction::make_node("0"), "1");
  AddSpecialValue(UserFunction::make_node("2*pi*i"), "1");
  AddSpecialValue(UserFunction::make_node("pi*i"), "-1");
  AddSpecialValue(UserFunction::make_node("1/2*pi*i"), "i");
  AddSpecialValue(UserFunction::make_node("3/2*pi*i"), "-i");
};

} // namespace Equation
