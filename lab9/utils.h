#ifndef PDP_UTILS_
#define PDP_UTILS_

#include <bits/stdc++.h>
#include <mpi.h>

#include "polynomial.h"

void MPI_RecvVector(std::vector<int>* vector, int source) {
  MPI_Status status;

  int num_digits;
  int* coeffs;
  MPI_Recv(&num_digits, 1, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  coeffs = new int[num_digits];
  MPI_Recv(coeffs, num_digits, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD,
           &status);

  *vector = std::vector<int>(coeffs, coeffs + num_digits);

  delete coeffs;
}

void MPI_SendVector(std::vector<int>& vector, int source) {
  int num_digits = vector.size();
  int* coeffs = vector.data();
  MPI_Send(&num_digits, 1, MPI_INT, source, 123, MPI_COMM_WORLD);
  MPI_Send(coeffs, num_digits, MPI_INT, source, 123, MPI_COMM_WORLD);
}

void MPI_RecvPolynomial(Polynomial* polynomial, int source) {
  std::vector<int> coeffs;
  MPI_RecvVector(&coeffs, source);
  *polynomial = Polynomial(coeffs);
}

void MPI_SendPolynomial(Polynomial& polynomial, int source) {
  std::vector<int> coeffs = polynomial.all();
  MPI_SendVector(coeffs, source);
}

#endif  // PDP_UTILS_
