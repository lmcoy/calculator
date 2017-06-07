#include "Equation.hpp"

std::string eval(const std::string &expression) {
  Equation::Equation eq;
  eq.Set(expression);
  return eq.Evaluate(true);
}

int main(int argc, char *argv[]) {
  if (argc >= 3) {
    std::cerr << "error: too many arguments\n";
    return 1;
  }
  if (argc == 2) {
    std::cout << eval(argv[1]) << "\n";
    return 0;
  }
  while (true) {
    std::cout << "> ";
    std::string line;
    std::getline(std::cin, line);
    if (line == "exit" || line == "") {
      return 0;
    }
    std::cout << eval(line) << "\n";
  }
  return 0;
}
