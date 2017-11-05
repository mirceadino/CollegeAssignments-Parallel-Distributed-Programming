#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

class Matrix {
  public:
    Matrix(int N, int M):
      N_(N),
      M_(M),
      elements_(N, std::vector<int>(M, 0)),
      mutex_per_row_(N),
      num_computed_per_row_(N, 0) {
      for (int i = 0; i < N; ++i) {
        mutex_per_row_[i] = std::make_unique<std::mutex>();
      }
    }

    Matrix(int N, int M, bool computed):
      N_(N),
      M_(M),
      elements_(N, std::vector<int>(M, 0)),
      mutex_per_row_(N), 
      num_computed_per_row_(N, 0) {
      if (computed) {
        for (int i = 0; i < N; ++i) {
          mutex_per_row_[i] = std::make_unique<std::mutex>();
          num_computed_per_row_[i] = M;
        }
      }
    }

    int N() const {
      return N_;
    }

    int M() const {
      return M_;
    }

    void set(int i, int j, int val) {
      elements_[i][j] = val;
    }

    void add(int i, int j, int val) {
      elements_[i][j] += val;
    }

    int get(int i, int j) {
      return elements_[i][j];
    }

    void set_computed(int i) {
      std::lock_guard<std::mutex> lock_per_row(*mutex_per_row_[i]);
      ++num_computed_per_row_[i];
    }

    int num_computed_per_row(int i) {
      std::lock_guard<std::mutex> lock_per_row(*mutex_per_row_[i]);
      return num_computed_per_row_[i];
    }

  private:
    const int N_;
    const int M_;
    std::vector<std::vector<int>> elements_;
    std::vector<std::unique_ptr<std::mutex>> mutex_per_row_;
    std::vector<int> num_computed_per_row_;
};
