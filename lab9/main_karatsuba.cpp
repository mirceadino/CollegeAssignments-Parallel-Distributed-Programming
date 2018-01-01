#include <bits/stdc++.h>
#include <mpi.h>

#include "polynomial.h"
#include "utils.h"

using namespace std;

int kMe, kNumNodes;

Polynomial ReadPolynomial();
void Master();
void Slave();
Polynomial Multiply(const Polynomial&, const Polynomial&, bool);

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

  Polynomial C = Multiply(A, B, true);
  for (int i = 0; i < C.size(); ++i) {
    cout << C[i] << " ";
  }
  cout << endl;
}

void Slave() {
  int source = (kMe - 1) / 2;

  Polynomial A, B;
  MPI_RecvPolynomial(&A, source);
  MPI_RecvPolynomial(&B, source);

  Polynomial C = Multiply(A, B, true);

  MPI_SendPolynomial(C, source);
}

Polynomial Multiply(const Polynomial& lhs, const Polynomial& rhs,
                    bool distributed) {
  if (lhs.size() == 1) {
    return rhs * lhs[0];
  } else if (rhs.size() == 1) {
    return lhs * rhs[0];
  }

  const int m = std::max(lhs.size(), rhs.size()) / 2;
  Polynomial b = lhs.split(0, m);
  Polynomial a = lhs.split(m, lhs.size());
  Polynomial d = rhs.split(0, m);
  Polynomial c = rhs.split(m, rhs.size());

  const int ac_node = ((2 * kMe + 1) < kNumNodes) ? (2 * kMe + 1) : -1;
  const int bd_node = ((2 * kMe + 2) < kNumNodes) ? (2 * kMe + 2) : -1;

  if (distributed && ac_node != -1) {
    MPI_SendPolynomial(a, 2 * kMe + 1);
    MPI_SendPolynomial(c, 2 * kMe + 1);
  }

  if (distributed && bd_node != -1) {
    MPI_SendPolynomial(b, 2 * kMe + 2);
    MPI_SendPolynomial(d, 2 * kMe + 2);
  }

  Polynomial ac;
  if (distributed && ac_node != -1) {
    MPI_RecvPolynomial(&ac, 2 * kMe + 1);
  } else {
    ac = Multiply(a, c, false);
  }

  Polynomial bd;
  if (distributed && bd_node != -1) {
    MPI_RecvPolynomial(&bd, 2 * kMe + 2);
  } else {
    bd = Multiply(b, d, false);
  }

  Polynomial abcd = Multiply(a + b, c + d, false);

  std::vector<int> ac_v = ac.all();
  reverse(ac_v.begin(), ac_v.end());
  ac_v.resize(ac.size() + 2 * m);
  reverse(ac_v.begin(), ac_v.end());

  Polynomial q = abcd - ac - bd;
  std::vector<int> q_v = q.all();
  reverse(q_v.begin(), q_v.end());
  q_v.resize(q.size() + m);
  reverse(q_v.begin(), q_v.end());

  return Polynomial(ac_v) + Polynomial(q_v) + bd;
}
