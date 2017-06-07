#include "gtest/gtest.h"

#include "Equation.hpp"

std::string eval(const std::string &expression) {
  Equation::Equation eq;
  eq.Set(expression);
  return eq.Evaluate();
}

std::string evalf(const std::string &expression) {
  Equation::Equation eq;
  eq.Set(expression);
  return eq.Evaluate(true);
}

Equation::Equation Eq(const std::string &expr) {
  Equation::Equation eq;
  eq.Set(expr);
  eq.Evaluate();
  return eq;
}

#define EQUATION_EXPECT_EQUAL(a, b)                                            \
  EXPECT_TRUE(Eq(a) == Eq(b))                                                  \
      << "first: " << Eq(a).ToString() << ", second: " << Eq(b).ToString();

TEST(Equation, Numbers) {
  EXPECT_EQ(eval("4"), "4");
  EXPECT_EQ(eval("-4"), "-4");
  EXPECT_EQ(eval("132"), "132");
  EXPECT_EQ(eval("0"), "0");
  EXPECT_DOUBLE_EQ(atof(eval("-54.2").c_str()), -54.2);
  EXPECT_DOUBLE_EQ(atof(eval("-54.2e2").c_str()), -54.2e2);
}

TEST(Equation, Addition) {
  EXPECT_EQ(eval("4+3"), "7");
  EXPECT_EQ(eval("4-3"), "1");
  EXPECT_EQ(eval("-4+3-2"), "-3");
  EXPECT_EQ(eval("48-32+6-1+0"), "21");
  EXPECT_EQ(eval("1204-30-1204+30"), "0");
}

TEST(Equation, Multiplication) {
  EXPECT_EQ(eval("4*3"), "12");
  EXPECT_EQ(eval("4/3"), "4 / 3");
  EXPECT_EQ(eval("4/2"), "2");
  EXPECT_EQ(eval("2/6"), "1 / 3");
  EXPECT_EQ(eval("2/6 * 5"), "5 / 3");
}

TEST(Equation, AdditionOfFractions) {
  EXPECT_EQ(eval("1/2+1/2"), "1");
  EXPECT_EQ(eval("1/3+1/2"), "5 / 6");
}

TEST(Equation, SubtractionOfFractions) {
  EXPECT_EQ(eval("1/2-1/2"), "0");
  EXPECT_EQ(eval("1/3-1/2"), "-1 / 6");
  EXPECT_DOUBLE_EQ(atof(eval("1/3-0.5").c_str()), 1.0 / 3.0 - 0.5);
}

TEST(Equation, Power) {
  EQUATION_EXPECT_EQUAL("x*x", "x ^ 2");
  EQUATION_EXPECT_EQUAL("4 * 3*x*x", "12 * x ^ 2");
  EQUATION_EXPECT_EQUAL("x/x", "1");
  EQUATION_EXPECT_EQUAL("(x+1)*(x+1)*(x+1)", "(x+1)^3");
  EQUATION_EXPECT_EQUAL("1/x/x", "1/x^2");
  EQUATION_EXPECT_EQUAL("1/(x*x)", "1/x^2");
  EQUATION_EXPECT_EQUAL("x*x*y*y*y", "x ^ 2 * y^3");
  EQUATION_EXPECT_EQUAL("x*x*x^6/x^4", "x ^ 4");
  EQUATION_EXPECT_EQUAL("x^a*x^b", "x ^ (a+b)");
  EQUATION_EXPECT_EQUAL("4*x", "x*4");
}

TEST(Equation, Latex) {
  EXPECT_EQ(Eq("x / y").ToLatex(), "\\frac{x}{y}");
  EXPECT_EQ(Eq("x / y^3").ToLatex(), "\\frac{x}{y^{3}}");
  EXPECT_EQ(Eq("x / (y+1)^3 * 6 /3").ToLatex(),
            "\\frac{2x}{\\left(y + \\frac{1}{1}\\right)^{3}}");
  EXPECT_EQ(Eq("x^(-1)").ToLatex(), "x^{-1}");
  EXPECT_EQ(Eq("2^-1").ToLatex(), "\\frac{1}{2}");
  EXPECT_EQ(Eq("x/y/z/a").ToLatex(), "\\frac{x}{y z a}");
}

TEST(Equation, SinSpecialValues) {
  EQUATION_EXPECT_EQUAL("sin(pi)", "0");
  EQUATION_EXPECT_EQUAL("sin(2*pi)", "0");
  EQUATION_EXPECT_EQUAL("sin(pi*2)", "0");
  EQUATION_EXPECT_EQUAL("sin(pi/2)", "1");
  EQUATION_EXPECT_EQUAL("sin(3/2*pi)", "-1");
  EQUATION_EXPECT_EQUAL("sin(4/2*pi)", "0");
}

TEST(Equation, Sin) {
  EXPECT_EQ(eval("sin(1)"), "sin(1)");
  EXPECT_EQ(eval("sin(pi+1)"), "sin(pi + 1)");
}

TEST(Equation, SinNumeric) {
  EXPECT_DOUBLE_EQ(atof(evalf("sin(1)").c_str()), sin(1));
  EXPECT_DOUBLE_EQ(atof(evalf("sin(pi+0.1)").c_str()), sin(M_PI + 0.1));
}

TEST(Equation, CosSpecialValues) {
  EQUATION_EXPECT_EQUAL("cos(0)", "1");
  EQUATION_EXPECT_EQUAL("cos(pi)", "-1");
  EQUATION_EXPECT_EQUAL("cos(2*pi)", "1");
  EQUATION_EXPECT_EQUAL("cos(pi*2)", "1");
  EQUATION_EXPECT_EQUAL("cos(pi/2)", "0");
  EQUATION_EXPECT_EQUAL("cos(3/2*pi)", "0");
  EQUATION_EXPECT_EQUAL("cos(4/2*pi)", "1");
}

TEST(Equation, Cos) {
  EXPECT_EQ(eval("cos(1)"), "cos(1)");
  EXPECT_EQ(eval("cos(pi+1)"), "cos(pi + 1)");
}

TEST(Equation, CosNumeric) {
  EXPECT_DOUBLE_EQ(atof(evalf("cos(1)").c_str()), cos(1));
  EXPECT_DOUBLE_EQ(atof(evalf("cos(pi+0.1)").c_str()), cos(M_PI + 0.1));
}
