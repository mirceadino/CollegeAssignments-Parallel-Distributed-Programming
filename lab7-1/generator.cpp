#include <bits/stdc++.h>

using namespace std;

int kSeed;
int kN, kM;
int kMinVal;
int kMaxVal;

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr,
            "usage: %s <n> <min> <max> [<seed>]\n",
            argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &kN);
  sscanf(argv[2], "%d", &kMinVal);
  sscanf(argv[3], "%d", &kMaxVal);

  if (argc >= 5) {
    sscanf(argv[4], "%d", &kSeed);
  } else {
    kSeed = clock();
  }

  srand(kSeed);

  vector<int> A(kN);
  for (int i = 0; i < kN; ++i) {
    A[i] = rand() % (kMaxVal - kMinVal + 1) + kMinVal;
  }


  fprintf(stderr, "Seed: %d\n", kSeed);
  printf("%d\n", kN);
  for (int i = 0; i < kN; ++i) {
    printf("%d ", A[i]);
  }
  printf("\n");

  return 0;
}
