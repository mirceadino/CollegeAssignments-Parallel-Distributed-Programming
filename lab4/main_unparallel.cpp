#include <bits/stdc++.h>

#include "matrix.h"

using namespace std;

void ComputeLines(Matrix& A, Matrix& B, vector<int> lines, Matrix* C) {
  const int M = A.M();
  const int P = B.M();
  for (const int i : lines) {
    fprintf(stderr, "Computing line %d...\n", i);
    for (int j = 0; j < P; ++j) {
      for (int k = 0; k < M; ++k) {
        while (A.num_computed_per_row(i) <= k) {}
        C->add(i, j, A.get(i, k) * B.get(k, j));
      }
      C->set_computed(i);
    }
    fprintf(stderr, "Line %d computed!\n", i);
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
  cerr << "Using " << num_threads << " threads per multiplication." << endl;

  int N, M, P, Q;
  cerr << "Read matrices' sizes N, M, P, Q." << endl;
  cerr << "First matrix is NxM. ";
  cerr << "Second matrix is MxP. ";
  cerr << "Second matrix is PxQ. ";
  cerr << "Result will be NxQ." << endl;
  cin >> N >> M >> P >> Q;

  cerr << "Read first matrix: N*M elements:" << endl;
  Matrix first_matrix(N, M, true);
  for (int i = 0; i < N; ++i) {
    for (int j = 0, x; j < M; ++j) {
      cin >> x;
      first_matrix.set(i, j, x);
    }
  }

  cerr << "Read second matrix: M*P elements:" << endl;
  Matrix second_matrix(M, P, true);
  for (int i = 0; i < M; ++i) {
    for (int j = 0, x; j < P; ++j) {
      cin >> x;
      second_matrix.set(i, j, x);
    }
  }

  cerr << "Read third matrix: P*Q elements:" << endl;
  Matrix third_matrix(P, Q, true);
  for (int i = 0; i < P; ++i) {
    for (int j = 0, x; j < Q; ++j) {
      cin >> x;
      third_matrix.set(i, j, x);
    }
  }

  vector<future<void>> futures;
  Matrix tmp_matrix(N, P);
  Matrix result(N, Q);
  const int lines_per_thread = N / num_threads + (N % num_threads != 0);
  for (int i = 0; i < num_threads; ++i) {
    vector<int> lines;
    for (int k = i * lines_per_thread; k < min(N, (i + 1)*lines_per_thread); ++k) {
      lines.push_back(k);
    }
    futures.push_back(async(launch::async,
                            ComputeLines, ref(first_matrix),
                            ref(second_matrix), lines, &tmp_matrix));
  }
  for (future<void>& future : futures) {
    future.wait();
  }
  futures.clear();
  for (int i = 0; i < num_threads; ++i) {
    vector<int> lines;
    for (int k = i * lines_per_thread; k < min(N, (i + 1)*lines_per_thread); ++k) {
      lines.push_back(k);
    }
    futures.push_back(async(launch::async,
                            ComputeLines, ref(tmp_matrix),
                            ref(third_matrix), lines, &result));
  }
  for (future<void>& future : futures) {
    future.wait();
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < Q; ++j) {
      cout << result.get(i, j) << " ";
    }
    cout << endl;
  }

  return 0;
}
