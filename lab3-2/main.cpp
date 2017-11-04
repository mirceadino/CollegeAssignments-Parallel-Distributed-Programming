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
        C->add(i, j, A.get(i, k) * B.get(k, j));
      }
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
  cerr << "Using " << num_threads << " threads." << endl;

  int N, M, P;
  cerr << "Read matrices' sizes N, M, P." << endl;
  cerr << "First matrix is NxM. ";
  cerr << "Second matrix is MxP. ";
  cerr << "Result will be NxP." << endl;
  cin >> N >> M >> P;

  cerr << "Read first matrix: N*M elements:" << endl;
  Matrix first_matrix(N, M);
  for (int i = 0; i < N; ++i) {
    for (int j = 0, x; j < M; ++j) {
      cin >> x;
      first_matrix.set(i, j, x);
    }
  }

  cerr << "Read second matrix: M*P elements:" << endl;
  Matrix second_matrix(M, P);
  for (int i = 0; i < M; ++i) {
    for (int j = 0, x; j < P; ++j) {
      cin >> x;
      second_matrix.set(i, j, x);
    }
  }

  vector<future<void>> futures;
  Matrix result(N, P);
  const int lines_per_thread = N / num_threads + (N % num_threads != 0);
  for (int i = 0; i < num_threads; ++i) {
    vector<int> lines;
    for (int k = i * lines_per_thread; k < min(N, (i + 1)*lines_per_thread); ++k) {
      lines.push_back(k);
    }
    futures.push_back(async(launch::async,
                            ComputeLines, ref(first_matrix), 
                            ref(second_matrix), lines, &result));
  }
  for (future<void>& future : futures) {
    future.wait();
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < P; ++j) {
      cout << result.get(i, j) << " ";
    }
    cout << endl;
  }

  return 0;
}
