#ifndef KV_URAVNENIE_POLYNOMIAL_H
#define KV_URAVNENIE_POLYNOMIAL_H

#include <vector>

/**
 * @brief Корень уравнения
 *
 * В объекте этого класса хранится корень уравнения и его алгебраическая кратность.
 *
 * @tparam T Тип данных корня
 */
template <typename T>
class Root {

 public:

  Root() = delete;

  explicit Root(T value, size_t multiplicity = 1) {
    value_ = value;
    multiplicity_ = multiplicity;
  }

  Root(const Root& other) = default;

  Root(Root&& other) noexcept = default;

  Root& operator =(const Root& other) {
    if (this != &other) {
      value_ = other.value_;
      multiplicity_ = other.multiplicity_;
    }
    return *this;
  }

  Root& operator =(Root&& other) noexcept {
    if (this != &other) {
      value_ = std::move(other.value_);
      multiplicity_ = other.multiplicity_;
    }
    return *this;
  }

  ~Root() = default;

  bool operator ==(const Root& other) = delete;

  const T& GetValue() const {
    return value_;
  }

  size_t GetMultiplicity() const {
    return multiplicity_;
  }

 private:

  T value_;
  size_t multiplicity_;
};

/**
 * @brief Многочлен
 *
 * В объекте этого класса хранятся коэффициенты многочлена
 *
 * @warning Коэффициент при младшей степени имеет индекс 0
 *
 * @tparam T Тип данных коэффициентов
 */
template <typename T>
class Polynomial {

  /**
   *
   * Функция избавляет от ведущих нулей и поддерживает инвариант наличия хотя бы одного коэффициента
   */
  void NormalizeCoefficients() {
    for (size_t i = coefficients_.size() - 1; i > 0; --i) {
      if (coefficients_[i] == IDENTITY_ELEMENT) {
        coefficients_.pop_back();
      } else {
        break;
      }
    }
  }

 public:

  Polynomial() {
    coefficients_.push_back(IDENTITY_ELEMENT);
  }

  explicit Polynomial(const std::vector<T>& coefficients) {
    coefficients_ = coefficients;
    if (!coefficients_.size()) {
      coefficients_.push_back(IDENTITY_ELEMENT);
    }
    NormalizeCoefficients();
  }

  Polynomial(const Polynomial& other) : coefficients_(other.coefficients_) {}

  Polynomial(Polynomial&& other) noexcept {
    coefficients_.swap(other.coefficients_);
  }

  Polynomial& operator =(const Polynomial& other) {
    if (this != &other) {
      coefficients_ = other;
    }
    return *this;
  }

  Polynomial& operator =(Polynomial&& other) noexcept {
    if (this != &other) {
      coefficients_.swap(other.coefficients_);
    }
    return *this;
  }

  ~Polynomial() = default;

  bool operator ==(const Polynomial& other) const {
    return coefficients_ == other.coefficients_;
  }

  /**
   *
   * @return Степень многочлена
   */
  size_t GetDegree() const {
    return coefficients_.size() - 1;
  }

  /**
   * Обновляет значение коэффициента
   * @param index Индекс коэффициента
   * @param value Новое значение коэффициента
   */
  void SetCoefficient(size_t index, const T& value) {
    if (index >= coefficients_.size()) {
      coefficients_.resize(index + 1, IDENTITY_ELEMENT);
    }
    coefficients_[index] = value;
    NormalizeCoefficients();
  }

  /**
   * Получить значение коэффициента по индексу
   * @param index Индекс коэффициента
   * @return Значение коэффициента
   */
  const T& GetCoefficient(size_t index) const {
    return coefficients_.at(index);
  }

  /**
   * Получить значение коэффициента по индексу
   * @param index Индекс коэффициента
   * @return Значение коэффициента
   */
  const T& operator [](size_t index) const {
    return coefficients_.at(index);
  }

  /**
   * Проверка, является ли значение корнем этого многочлена
   * @param root Проверяемое значение
   * @return True, если является. False иначе.
   */
  bool HasRoot(const Root<T>& root) const {
    T sum = coefficients_[0];
    T current_power = root.GetValue();
    for (size_t i = 1; i < coefficients_.size(); ++i) {
      sum += current_power * coefficients_[i];
      current_power *= root.GetValue();
    }
    return sum == 0;
  }

 private:

  const static T IDENTITY_ELEMENT;
  std::vector<T> coefficients_;
};

template <typename T>
const T Polynomial<T>::IDENTITY_ELEMENT = static_cast<T>(0);

#endif //KV_URAVNENIE_POLYNOMIAL_H
