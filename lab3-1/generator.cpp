#include <bits/stdc++.h>

using namespace std;

int kSeed;
int kN, kM, kP;
int kMinVal;
int kMaxVal;

int main(int argc, char** argv) {
  if (argc < 6) {
    fprintf(stderr,
            "usage: %s <n> <m> <p> <min> <max> [<seed>]\n",
            argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &kN);
  sscanf(argv[2], "%d", &kM);
  sscanf(argv[3], "%d", &kP);
  sscanf(argv[4], "%d", &kMinVal);
  sscanf(argv[5], "%d", &kMaxVal);

  if (argc >= 7) {
    sscanf(argv[6], "%d", &kSeed);
  } else {
    kSeed = clock();
  }

  srand(kSeed);

  vector<vector<int>> A(kN, vector<int>(kM)), B(kM, vector<int>(kP));
  for (int i = 0; i < kN; ++i) {
    for (int j = 0; j < kM; ++j) {
      A[i][j] = rand() % (kMaxVal - kMinVal) + kMinVal;
    }
  }
  for (int i = 0; i < kM; ++i) {
    for (int j = 0; j < kP; ++j) {
      B[i][j] = rand() % (kMaxVal - kMinVal) + kMinVal;
    }
  }


  fprintf(stderr, "Seed: %d\n", kSeed);
  printf("%d %d %d\n", kN, kM, kP);
  for (int i = 0; i < kN; ++i) {
    for (int j = 0; j < kM; ++j) {
      printf("%d ", A[i][j]);
    }
    printf("\n");
  }
  for (int i = 0; i < kM; ++i) {
    for (int j = 0; j < kP; ++j) {
      printf("%d ", B[i][j]);
    }
    printf("\n");
  }

  return 0;
}
