#ifndef KV_URAVNENIE_EQUATIONSOLVER_H
#define KV_URAVNENIE_EQUATIONSOLVER_H

#include "Polynomial.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <limits>

#ifdef TESTING_MODE
#define COUT std::cout
#else
#define COUT if (0) std::cout
#endif

/**
 * @brief Неподдерживаемая степень многочлена
 *
 * Класс-исключение для ситуаций, когда EquationSolver не умеет решать уравнение данной степени
 */
class UnsupportedPolynomiaDegreeException : public std::exception {};

/**
 * @brief Решение уравнения
 *
 * Объект данного класса хранит корни
 *
 * @tparam T Тип данных ответа
 */
template <typename T>
class EquationSolution {
  std::vector<Root<T>> roots_;

 public:
  EquationSolution() = default;
  ~EquationSolution() = default;

  /**
   * Получить все корни
   * @return Все корни
   */
  const std::vector<Root<T>>& GetRoots() const {
    return roots_;
  }

  /**
   * Добавить новый корень к текущему списку корней
   * @param root Новый корень
   */
  void AddRoot(const Root<T>& root) {
    roots_.push_back(root);
  }

  /**
   * Добавить новый корень к текущему списку корней
   * @param root Новый корень
   */
  void AddRoot(Root<T>&& root) {
    roots_.push_back(std::move(root));
  }
};

/**
 * @brief Решатель уравнения
 *
 * @tparam T Тип данных коэффициентов решаемого уравнения
 */
template <typename T>
class EquationSolver;

/**
 * @brief Спецификация для уравнений с вещественными коэффициентами
 *
 * @warning Класс реализован как синглтон
 */
template <>
class EquationSolver<double> {

  const static double EPS;

  EquationSolver() = default;
  ~EquationSolver() = default;

  auto SolveLinear(const Polynomial<double>& polynomial) const {
    assert(polynomial.GetDegree() == 1);

    double rootValue = -polynomial.GetCoefficient(0) / polynomial.GetCoefficient(1);
    Root<double> root(rootValue, 1);

    EquationSolution<double> solution;
    solution.AddRoot(root);
    return solution;
  }

  auto SolveQuadratic(const Polynomial<double>& polynomial) const {
    assert(polynomial.GetDegree() == 2);

    double a = polynomial.GetCoefficient(2);
    double b = polynomial.GetCoefficient(1);
    double c = polynomial.GetCoefficient(0);
    double D = b * b - 4 * a * c;

    EquationSolution<double> solution;

    if (fabs(D) <= EPS) {

      COUT << "# Дискриминант равен нулю...\n";

      Root<double> root(-b / (2 * a), 2);
      solution.AddRoot(root);

    } else if (D > EPS) {

      COUT << "# Дискриминант больше нуля...\n";

      double sqrtD = sqrt(D);
      Root<double> root1((-b + sqrtD) / (2 * a), 1);
      Root<double> root2((-b - sqrtD) / (2 * a), 1);
      solution.AddRoot(root1);
      solution.AddRoot(root2);

    } else {
      COUT << "# Дискриминант меньше нуля...\n";
    }

    return solution;
  }

 public:

  EquationSolver(EquationSolver const&) = delete;
  EquationSolver& operator =(EquationSolver const&) = delete;

  /**
   * Получить экземляр синлгтона решателя
   * @return
   */
  static EquationSolver& GetInstance() {
    static EquationSolver equationSolver;
    return equationSolver;
  }

  /**
   * Ищет корни многочлена
   *
   * @warning На данный момент поддерживаются многочлены степени не более 2
   *
   * @param polynomial Многочлен
   * @return Корни
   */
  EquationSolution<double> Solve(const Polynomial<double>& polynomial) const {
    EquationSolution<double> solution;
    switch (polynomial.GetDegree()) {
      case 0:
        COUT << "# Решаю уравнение степени 0...\n";
        if (fabs(polynomial.GetCoefficient(0)) < EPS) {
          solution.AddRoot(Root<double>(std::numeric_limits<double>::infinity(), 1));
        }
        return solution;
      case 1:
        COUT << "# Решаю линейное уравнение...\n";
        return SolveLinear(polynomial);
      case 2:
        COUT << "# Решаю квадратное уравнение...\n";
        return SolveQuadratic(polynomial);
      default:
        COUT << "# Не умею такое решать...\n";
        throw UnsupportedPolynomiaDegreeException();
    }
  }

};

const double EquationSolver<double>::EPS = 1e-6;

/**
 * Вывести на экран все корни
 * @tparam T Тип данных
 * @param solution Корни для печати
 */
template <typename T>
void PrintRoots(const EquationSolution<T>& solution) {
  const auto& roots = solution.GetRoots();

  if (roots.empty()) {
    std::cout << "NO\n";
    return;
  }

  for (const Root<T>& root: roots) {
    COUT << "Корень: ";
    std::cout << root.GetValue() << " ";
    COUT << "Кратность: ";
    std::cout << root.GetMultiplicity() << "\n";
  }
}

#endif //KV_URAVNENIE_EQUATIONSOLVER_H
