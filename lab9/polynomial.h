#ifndef PDP_POLYNOMIAL_
#define PDP_POLYNOMIAL_

#include <bits/stdc++.h>
#include <mpi.h>

class Polynomial {
  public:
    Polynomial() {}
    Polynomial(const std::vector<int>& coeffs): coeffs_(coeffs) {
      while (!coeffs_.empty() && !coeffs_.back()) {
        coeffs_.pop_back();
      }
      if (coeffs_.empty()) {
        coeffs_.push_back(0);
      }
    }

    const int& operator[](const int& i) const {
      return coeffs_[i];
    }

    int size() const {
      return coeffs_.size();
    }

    std::vector<int> all() const {
      return coeffs_;
    }

    Polynomial split(int lo, int hi) const {
      return Polynomial(std::vector<int>(coeffs_.begin() + std::max(0, lo),
                                         coeffs_.begin() + std::min(hi, size())));
    }

    Polynomial operator*(const int& rhs) const {
      std::vector<int> result(coeffs_);
      for (int& el : result) {
        el *= rhs;
      }
      return Polynomial(result);
    }

    Polynomial operator+(const Polynomial& rhs) const {
      const Polynomial& lhs = *this;
      std::vector<int> result(std::max(lhs.size(), rhs.size()));
      for (int i = 0; i < (int)std::min(rhs.size(), lhs.size()); ++i) {
        result[i] = lhs[i] + rhs[i];
      }
      for (int i = std::min(rhs.size(), lhs.size()); i < (int)lhs.size(); ++i) {
        result[i] = lhs[i];
      }
      for (int i = std::min(rhs.size(), lhs.size()); i < (int)rhs.size(); ++i) {
        result[i] = rhs[i];
      }
      while (!result.empty() && !result.back()) {
        result.pop_back();
      }
      return Polynomial(result);
    }

    Polynomial operator-(const Polynomial& rhs) const {
      const Polynomial& lhs = *this;
      std::vector<int> result(std::max(lhs.size(), rhs.size()));
      for (int i = 0; i < (int)std::min(rhs.size(), lhs.size()); ++i) {
        result[i] = lhs[i] - rhs[i];
      }
      for (int i = std::min(rhs.size(), lhs.size()); i < (int)lhs.size(); ++i) {
        result[i] = lhs[i];
      }
      for (int i = std::min(rhs.size(), lhs.size()); i < (int)rhs.size(); ++i) {
        result[i] = -rhs[i];
      }
      while (!result.empty() && !result.back()) {
        result.pop_back();
      }
      return Polynomial(result);
    }

  private:
    std::vector<int> coeffs_;
};


class PolynomialMultiplication {
  public:
    PolynomialMultiplication() {}
    virtual ~PolynomialMultiplication() {}

    virtual Polynomial Multiply(const Polynomial& lhs,
                                const Polynomial& rhs,
                                bool distributed) = 0;
};

#endif  // PDP_POLYNOMIAL_
