CMD line Calculator
===================

simple cmd line calculator

[![Build Status](https://travis-ci.org/lmcoy/calculator.svg?branch=master)](https://travis-ci.org/lmcoy/calculator)

calculator
----------

`calculator` takes an expression as argument and prints the result after
evaluation to stdout. If no argument is given, calculator reads an
expression from stdin. An empty line or the keyword "exit" stops the
evaluation.

`calculator` does not evaluate functions that lead to inaccurate floating
point results, e.g. "sin(1)" is not evaluated but "sin(pi)" is evaluated
because the precise result is known. If you want to evaluate expression
like "sin(1)" numerically, use `calculatorf`.

*Examples:*
```
$ ./calculator
> 5+6
11
> 7/2
7 / 2
> sin(1)
sin(1)
> sin(0)
0
> 5*(3+2)
25
> x*x*x
x ^ 3
> 5^(1/2)
5 ^ 1 / 2
> pi
pi
> 8/4
2
> 3^2
9
```

calculatorf
-----------

`calculatorf` does the same as `calculator` but evaluates
expressions numerically.

*Examples:*
```
$ ./calculatorf
> 5+6
11
> 7/2
3.5
> sin(1)
0.8414709848078965
> sin(0)
0
> 5*(3+2)
25
> x*x*x
x ^ 3
> 5^(1/2)
2.2360679774997898
> pi
3.1415926535897931
> 8/4
2
> 3^2
9
```
