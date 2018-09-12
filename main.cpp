#define TESTING_MODE

#include "EquationSolver.h"

#ifdef TESTING_MODE
#define COUT std::cout
#else
#define COUT if (0) std::cout
#endif

int main(int argc, const char **argv) {
  setlocale(LC_ALL, "Russian");

  double a, b, c;

  COUT << "# Здравствуйте!\n";
  COUT << "# Я помогу вам решить уравнение a * x^2 + b * x + c = 0\n";
  COUT << "# Введите a: ";
  std::cin >> a;
  COUT << "# Введите b: ";
  std::cin >> b;
  COUT << "# Введите c: ";
  std::cin >> c;

  Polynomial<double> polynomial({c, b, a});

  EquationSolver<double>& solver = EquationSolver<double>::GetInstance();
  PrintRoots(solver.Solve(polynomial));

  return 0;
}
