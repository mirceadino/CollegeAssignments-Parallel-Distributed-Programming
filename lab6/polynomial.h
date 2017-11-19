#ifndef PDP_POLYNOMIAL_
#define PDP_POLYNOMIAL_

#include <bits/stdc++.h>

template <class T>
class Polynomial {
  public:
    Polynomial() {}
    Polynomial(const std::vector<T>& coeffs): coeffs_(coeffs) {}

    const T& operator[](const int& i) const {
      return coeffs_[i];
    }

    int size() const {
      return coeffs_.size();
    }

  private:
    const std::vector<T> coeffs_;
};


template <class T>
class PolynomialMultiplication {
  public:
    PolynomialMultiplication() {}
    virtual ~PolynomialMultiplication() {}

    virtual Polynomial<T> Multiply(const Polynomial<T>& lhs,
                                   const Polynomial<T>& rhs) = 0;
};


template<class T>
class RegularSequentialPM: public PolynomialMultiplication<T> {
  public:
    RegularSequentialPM() {
      std::cerr << "Using regular sequential polynomial multiplication." << std::endl;
    }

    Polynomial<T> Multiply(const Polynomial<T>& lhs,
                           const Polynomial<T>& rhs) override {
      std::vector<T> result(lhs.size() + rhs.size() - 1);
      for (int i = 0; i < lhs.size(); ++i) {
        for (int j = 0; j < rhs.size(); ++j) {
          result[i + j] += lhs[i] * rhs[j];
        }
      }
      return Polynomial<T>(result);
    }
};


template<class T>
class RegularParallelPM: public PolynomialMultiplication<T> {
  public:
    RegularParallelPM(int num_threads): num_threads_(num_threads) {
      std::cerr << "Using regular parallel polynomial multiplication with " <<
                num_threads << " threads." << std::endl;
    }

    Polynomial<T> Multiply(const Polynomial<T>& lhs,
                           const Polynomial<T>& rhs) override {
      std::vector<std::future<void>> futures;
      std::vector<T> result(lhs.size() + rhs.size() - 1);
      const int coeffs_per_thread = result.size() / num_threads_ +
                                    (result.size() % num_threads_ != 0);
      for (int i = 0; i < num_threads_; ++i) {
        std::vector<int> coeffs;
        for (int k = i * coeffs_per_thread;
             k < std::min((int)result.size(), (i + 1)*coeffs_per_thread); ++k) {
          coeffs.push_back(k);
        }
        futures.push_back(std::async(std::launch::async,
                                     &RegularParallelPM::ComputeCoeffs,
                                     this,
                                     std::ref(lhs), std::ref(rhs),
                                     coeffs, &result));
      }
      for (std::future<void>& future : futures) {
        future.wait();
      }
      return Polynomial<T>(result);
    }

  private:
    void ComputeCoeffs(const Polynomial<T>& lhs, const Polynomial<T>& rhs,
                       std::vector<int> coeffs,
                       std::vector<T>* result) {
      for (const int i : coeffs) {
        for (int j = 0; j < lhs.size(); ++j) {
          if (i - j >= 0 && i - j < rhs.size()) {
            (*result)[i] += lhs[j] * rhs[i - j];
          }
        }
      }
    }

    const int num_threads_;
};


template<class T>
class KaratsubaSequentialPM: public PolynomialMultiplication<T> {
  public:
    KaratsubaSequentialPM() {
      std::cerr << "Using Karatsuba sequential polynomial multiplication." <<
                std::endl;
    }

    Polynomial<T> Multiply(const Polynomial<T>& lhs,
                           const Polynomial<T>& rhs) override {
      std::vector<T> result(lhs.size() + rhs.size() - 1);
      for (int i = 0; i < lhs.size(); ++i) {
        for (int j = 0; j < rhs.size(); ++j) {
          result[i + j] += lhs[i] * rhs[j];
        }
      }
      return Polynomial<T>(result);
    }
};


template<class T>
class KaratsubaParallelPM: public PolynomialMultiplication<T> {
  public:
    KaratsubaParallelPM(int num_threads): num_threads_(num_threads) {
      std::cerr << "Using Karatsuba parallel polynomial multiplication with " <<
                num_threads << " threads." << std::endl;
    }

    Polynomial<T> Multiply(const Polynomial<T>& lhs,
                           const Polynomial<T>& rhs) override {
      std::vector<std::future<void>> futures;
      std::vector<T> result(lhs.size() + rhs.size() - 1);
      const int coeffs_per_thread = result.size() / num_threads_ +
                                    (result.size() % num_threads_ != 0);
      for (int i = 0; i < num_threads_; ++i) {
        std::vector<int> coeffs;
        for (int k = i * coeffs_per_thread;
             k < std::min((int)result.size(), (i + 1)*coeffs_per_thread); ++k) {
          coeffs.push_back(k);
        }
        futures.push_back(std::async(std::launch::async,
                                     &KaratsubaParallelPM::ComputeCoeffs,
                                     this,
                                     std::ref(lhs), std::ref(rhs),
                                     coeffs, &result));
      }
      for (std::future<void>& future : futures) {
        future.wait();
      }
      return Polynomial<T>(result);
    }

  private:
    void ComputeCoeffs(const Polynomial<T>& lhs, const Polynomial<T>& rhs,
                       std::vector<int> coeffs,
                       std::vector<T>* result) {
      for (const int i : coeffs) {
        for (int j = 0; j < lhs.size(); ++j) {
          if (i - j >= 0 && i - j < rhs.size()) {
            (*result)[i] += lhs[j] * rhs[i - j];
          }
        }
      }
    }

    const int num_threads_;
};

#endif  // PDP_POLYNOMIAL_
