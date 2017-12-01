#include <bits/stdc++.h>

using namespace std;

int kSeed;
int kN, kM;
int kMinVal;
int kMaxVal;

int main(int argc, char** argv) {
  if (argc < 5) {
    fprintf(stderr,
            "usage: %s <n> <m> <min> <max> [<seed>]\n",
            argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &kN);
  sscanf(argv[2], "%d", &kM);
  sscanf(argv[3], "%d", &kMinVal);
  sscanf(argv[4], "%d", &kMaxVal);

  if (argc >= 6) {
    sscanf(argv[5], "%d", &kSeed);
  } else {
    kSeed = clock();
  }

  srand(kSeed);

  vector<int> A(kN);
  for (int i = 0; i < kN; ++i) {
    A[i] = rand() % (kMaxVal - kMinVal + 1) + kMinVal;
  }
  vector<int> B(kM);
  for (int i = 0; i < kM; ++i) {
    B[i] = rand() % (kMaxVal - kMinVal + 1) + kMinVal;
  }


  fprintf(stderr, "Seed: %d\n", kSeed);
  printf("%d\n", kN);
  for (int i = 0; i < kN; ++i) {
    printf("%d ", A[i]);
  }
  printf("\n");
  printf("%d\n", kM);
  for (int i = 0; i < kM; ++i) {
    printf("%d ", B[i]);
  }
  printf("\n");

  return 0;
}
