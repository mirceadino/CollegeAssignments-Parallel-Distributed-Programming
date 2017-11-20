#include <bits/stdc++.h>
#include "polynomial.h"

using namespace std;

Polynomial<int> ReadBigNumber() {
  string num;
  cin >> num;
  vector<int> coeffs;
  for (int i = num.size() - 1; i >= 0; --i) {
    coeffs.push_back(num[i] - '0');
  }
  return Polynomial<int>(coeffs);
}

Polynomial<int> ToBigNumber(const Polynomial<int> num) {
  vector<int> digits = num.all();
  int t = 0;
  for (int& digit : digits) {
    digit += t;
    t = digit / 10;
    digit %= 10;
  }
  while (t) {
    digits.push_back(t % 10);
    t /= 10;
  }
  return Polynomial<int>(digits);
}

int main(int argc, char** argv) {
  int num_threads = 1;
  int method = -1;
  if (argc < 3) {
    cerr << "usage: " << argv[0] << " <method> <num_threads>" << endl;
    cerr << "method 1 = regular O(N^2) unparallel" << endl;
    cerr << "method 2 = regular O(N^2) parallel [ <num_threads> is required here ]"
         << endl;
    cerr << "method 3 = Karatsuba unparallel" << endl;
    cerr << "method 4 = Karatsuba parallel" << endl;
    return 0;

  } else {
    sscanf(argv[1], "%d", &method);
    sscanf(argv[2], "%d", &num_threads);
    if (method < 1 || method > 4) {
      cerr << "Invalid method. Pick 1, 2, 3, or 4." << endl;
      return 0;
    }
  }

  Polynomial<int> A = ReadBigNumber();
  Polynomial<int> B = ReadBigNumber();

  unique_ptr<PolynomialMultiplication<int>> pol_mul;
  if (method == 1) {
    pol_mul = make_unique<RegularSequentialPM<int>>();
  } else if (method == 2) {
    pol_mul = make_unique<RegularParallelPM<int>>(num_threads);
  } else if (method == 3) {
    pol_mul = make_unique<KaratsubaSequentialPM<int>>();
  } else if (method == 4) {
    pol_mul = make_unique<KaratsubaParallelPM<int>>();
  }

  Polynomial<int> C = pol_mul->Multiply(A, B);
  C = ToBigNumber(C);

  for (int i = C.size() - 1; i >= 0; --i) {
    cout << C[i];
  }
  cout << endl;

  return 0;
}
