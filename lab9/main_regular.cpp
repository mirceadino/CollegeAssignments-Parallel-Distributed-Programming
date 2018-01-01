#include <bits/stdc++.h>
#include <mpi.h>

#include "polynomial.h"
#include "utils.h"

using namespace std;

int kMe, kNumNodes;

Polynomial ReadPolynomial();
void Master();
void Slave();

int main() {
  MPI_Init(0, 0);
  MPI_Comm_size(MPI_COMM_WORLD, &kNumNodes);
  MPI_Comm_rank(MPI_COMM_WORLD, &kMe);

  kMe ? Slave() : Master();

  MPI_Finalize();

  return 0;
}

Polynomial ReadPolynomial() {
  vector<int> coeffs;
  int size;
  cin >> size;
  for (int i = 0; i < size; ++i) {
    int x;
    cin >> x;
    coeffs.push_back(x);
  }
  return Polynomial(coeffs);
}

void Master() {
  Polynomial A = ReadPolynomial();
  Polynomial B = ReadPolynomial();
  std::vector<int> result(A.size() + B.size() - 1);
  const int coeffs_per_node = result.size() / kNumNodes +
                              (result.size() % kNumNodes != 0);

  vector<vector<int>> coeffs(kNumNodes);
  for (int node = 0; node < kNumNodes; ++node) {
    for (int k = node * coeffs_per_node;
         k < std::min((int)result.size(), (node + 1)*coeffs_per_node); ++k) {
      coeffs[node].push_back(k);
    }
    if (node != kMe) {
      MPI_SendPolynomial(A, node);
      MPI_SendPolynomial(B, node);
      MPI_SendVector(coeffs[node], node);
    }
  }

  for (const int i : coeffs[0]) {
    for (int j = 0; j < A.size(); ++j) {
      if (i - j >= 0 && i - j < B.size()) {
        result[i] += A[j] * B[i - j];
      }
    }
  }

  for (int node = 1; node < kNumNodes; ++node) {
    vector<int> partial_result;
    MPI_RecvVector(&partial_result, node);
    for (int i = 0; i < (int)partial_result.size(); ++i) {
      result[coeffs[node][i]] = partial_result[i];
    }
  }

  Polynomial C(result);
  for (int i = 0; i < C.size(); ++i) {
    cout << C[i] << " ";
  }
  cout << endl;
}

void Slave() {
  int source = 0;

  Polynomial A, B;
  MPI_RecvPolynomial(&A, source);
  MPI_RecvPolynomial(&B, source);

  vector<int> coeffs;
  MPI_RecvVector(&coeffs, source);

  vector<int> result;
  for (const int i : coeffs) {
    result.push_back(0);
    for (int j = 0; j < A.size(); ++j) {
      if (i - j >= 0 && i - j < B.size()) {
        result.back() += A[j] * B[i - j];
      }
    }
  }

  MPI_SendVector(result, source);
}
