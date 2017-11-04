#include <vector>

class Matrix {
  public:
    Matrix(int N, int M):
      N_(N),
      M_(M),
      elements_(N, std::vector<int>(M, 0)) {
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

  private:
    const int N_;
    const int M_;
    std::vector<std::vector<int>> elements_;
};
