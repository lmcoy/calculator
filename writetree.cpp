#include <fstream>
#include <iostream>

#include "Equation.hpp"

int main(int argc, char *argv[]) {
  if (argc >= 3) {
    std::cerr << "error: too many arguments\n";
    return 1;
  }
  if (argc < 2) {
    std::cerr << "error: need an expression as argument\n";
    return 1;
  }
  Equation::Equation eq;
  eq.Set(argv[1]);

  std::ofstream f1;
  f1.open("tree_before_eval.dot");
  if (!f1) {
    std::cerr << "error: could not open file\n";
    return 1;
  }
  eq.WriteInternalRepToStream(f1);
  f1.close();

  eq.Evaluate();
  f1.open("tree_after_eval.dot");
  if (!f1) {
    std::cerr << "error: could not open file\n";
    return 1;
  }
  eq.WriteInternalRepToStream(f1);
  f1.close();
  return 0;
}
