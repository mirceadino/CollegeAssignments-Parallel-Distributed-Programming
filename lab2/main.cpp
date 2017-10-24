#include <bits/stdc++.h>

using namespace std;

class Matrix {
  public:
    Matrix(int N, int M):
      N_(N),
      M_(M),
      elements_(N, vector<int>(M, 0)) {
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
    vector<vector<int>> elements_;
};

void ComputeLines(Matrix& A, Matrix& B, vector<int> lines, Matrix* C) {
  const int M = A.M();
  const int P = B.M();
  for (const int i : lines) {
    printf("Computing line %d...\n", i);
    for (int j = 0; j < P; ++j) {
      for (int k = 0; k < M; ++k) {
        C->add(i, j, A.get(i, k) * B.get(k, j));
      }
    }
    printf("Line %d computed!\n", i);
  }
}

int main(int argc, char* argv[]) {
  int num_threads = 1;
  if (argc < 2) {
    cerr << "Number of threads was not specified." << endl;
    cerr << "usage: " << argv[0] << " <num_threads>" << endl;
  } else {
    sscanf(argv[1], "%d", &num_threads);
  }
  cout << "Using " << num_threads << " threads." << endl;

  int N, M, P;
  cout << "Read matrices' sizes N, M, P." << endl;
  cout << "First matrix is NxM. ";
  cout << "Second matrix is MxP. ";
  cout << "Result will be NxP." << endl;
  cin >> N >> M >> P;

  cout << "Read first matrix: N*M elements:" << endl;
  Matrix first_matrix(N, M);
  for (int i = 0; i < N; ++i) {
    for (int j = 0, x; j < M; ++j) {
      cin >> x;
      first_matrix.set(i, j, x);
    }
  }

  cout << "Read second matrix: M*P elements:" << endl;
  Matrix second_matrix(M, P);
  for (int i = 0; i < M; ++i) {
    for (int j = 0, x; j < P; ++j) {
      cin >> x;
      second_matrix.set(i, j, x);
    }
  }

  vector<thread> thread_pool;
  thread_pool.reserve(num_threads);
  Matrix result(N, P);
  const int lines_per_thread = N / num_threads + (N % num_threads != 0);
  for (int i = 0; i < num_threads; ++i) {
    vector<int> lines;
    for (int k = i * lines_per_thread; k < min(N, (i + 1)*lines_per_thread); ++k) {
      lines.push_back(k);
    }
    thread_pool.push_back(thread(ComputeLines, ref(first_matrix),
                                 ref(second_matrix), lines,
                                 &result));
  }

  for (auto& thread : thread_pool) {
    thread.join();
  }

  cout << "Result is: " << endl;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < P; ++j) {
      cout << result.get(i, j) << " ";
    }
    cout << endl;
  }

  return 0;
}
