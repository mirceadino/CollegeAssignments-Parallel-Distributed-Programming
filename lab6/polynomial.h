#ifndef PDP_POLYNOMIAL_
#define PDP_POLYNOMIAL_

#include <bits/stdc++.h>

template <class T>
class Polynomial {
  public:
    Polynomial() {}
    Polynomial(const std::vector<T>& coeffs): coeffs_(coeffs) {
      while (!coeffs_.empty() && !coeffs_.back()) {
        coeffs_.pop_back();
      }
      if (coeffs_.empty()) {
        coeffs_.push_back(0);
      }
    }

    const T& operator[](const int& i) const {
      return coeffs_[i];
    }

    int size() const {
      return coeffs_.size();
    }

    std::vector<T> all() const {
      return coeffs_;
    }

    Polynomial<T> split(int lo, int hi) const {
      return Polynomial<T>(std::vector<T>(coeffs_.begin() + std::max(0, lo),
                                          coeffs_.begin() + std::min(hi, size())));
    }

    Polynomial<T> operator*(const T& rhs) const {
      std::vector<T> result(coeffs_);
      for (T& el : result) {
        el *= rhs;
      }
      return Polynomial<T>(result);
    }

    Polynomial<T> operator+(const Polynomial<T>& rhs) const {
      const Polynomial<T>& lhs = *this;
      std::vector<T> result(std::max(lhs.size(), rhs.size()));
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
      return Polynomial<T>(result);
    }

    Polynomial<T> operator-(const Polynomial<T>& rhs) const {
      const Polynomial<T>& lhs = *this;
      std::vector<T> result(std::max(lhs.size(), rhs.size()));
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
      return Polynomial<T>(result);
    }

  private:
    std::vector<T> coeffs_;
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
      if (lhs.size() == 1) {
        return rhs * lhs[0];
      } else if (rhs.size() == 1) {
        return lhs * rhs[0];
      }

      const int m = std::max(lhs.size(), rhs.size()) / 2;
      Polynomial<T> b = lhs.split(0, m);
      Polynomial<T> a = lhs.split(m, lhs.size());
      Polynomial<T> d = rhs.split(0, m);
      Polynomial<T> c = rhs.split(m, rhs.size());

      Polynomial<T> ac = Multiply(a, c);  // a * c
      Polynomial<T> bd = Multiply(b, d);  // b * d

      std::vector<T> ac_v = ac.all();
      reverse(ac_v.begin(), ac_v.end());
      ac_v.resize(ac.size() + 2 * m);
      reverse(ac_v.begin(), ac_v.end());

      Polynomial<T> abcd = Multiply(a + b, c + d);  // (a + b) * (c + d)
      Polynomial<T> q = abcd - ac - bd;
      std::vector<T> q_v = q.all();
      reverse(q_v.begin(), q_v.end());
      q_v.resize(q.size() + m);
      reverse(q_v.begin(), q_v.end());

      return Polynomial<T>(ac_v) + Polynomial<T>(q_v) + bd;
    }
};


template<class T>
class KaratsubaParallelPM: public PolynomialMultiplication<T> {
  public:
    KaratsubaParallelPM() {
      std::cerr << "Using Karatsuba parallel polynomial multiplication." << std::endl;
    }

    Polynomial<T> Multiply(const Polynomial<T>& lhs,
                           const Polynomial<T>& rhs) override {
      if (lhs.size() == 1) {
        return rhs * lhs[0];
      } else if (rhs.size() == 1) {
        return lhs * rhs[0];
      }

      const int m = std::max(lhs.size(), rhs.size()) / 2;
      Polynomial<T> b = lhs.split(0, m);
      Polynomial<T> a = lhs.split(m, lhs.size());
      Polynomial<T> d = rhs.split(0, m);
      Polynomial<T> c = rhs.split(m, rhs.size());

      // a * c
      std::future<Polynomial<T>> f_ac = std::async(std::launch::async,
                                        &KaratsubaParallelPM::Multiply, this,
                                        std::ref(a), std::ref(c));
      // b * d
      std::future<Polynomial<T>> f_bd = std::async(std::launch::async,
                                        &KaratsubaParallelPM::Multiply, this,
                                        std::ref(b), std::ref(d));
      // (a + b) * (c + d)
      std::future<Polynomial<T>> f_abcd = std::async(std::launch::async,
                                          &KaratsubaParallelPM::Multiply, this,
                                          a + b, c + d);

      f_ac.wait();
      f_bd.wait();
      f_abcd.wait();

      Polynomial<T> ac = f_ac.get();
      Polynomial<T> bd = f_bd.get();
      Polynomial<T> abcd = f_abcd.get();

      std::vector<T> ac_v = ac.all();
      reverse(ac_v.begin(), ac_v.end());
      ac_v.resize(ac.size() + 2 * m);
      reverse(ac_v.begin(), ac_v.end());

      Polynomial<T> q = abcd - ac - bd;
      std::vector<T> q_v = q.all();
      reverse(q_v.begin(), q_v.end());
      q_v.resize(q.size() + m);
      reverse(q_v.begin(), q_v.end());

      return Polynomial<T>(ac_v) + Polynomial<T>(q_v) + bd;
    }
};

#endif  // PDP_POLYNOMIAL_
