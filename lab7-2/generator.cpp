#include <bits/stdc++.h>

using namespace std;

int kSeed;
int kN, kM;

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr,
            "usage: %s <n> <m> [<seed>]\n",
            argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &kN);
  sscanf(argv[2], "%d", &kM);

  if (argc >= 4) {
    sscanf(argv[3], "%d", &kSeed);
  } else {
    kSeed = clock();
  }

  srand(kSeed);

  vector<vector<int>> nums(kN, vector<int>(kM));
  for (int i = 0; i < kN; ++i) {
    for (int j = 0; j < kM; ++j) {
      nums[i][j] = rand() % 10;
    }
  }

  fprintf(stderr, "Seed: %d\n", kSeed);
  printf("%d %d\n", kN, kM);
  for (int i = 0; i < kN; ++i) {
    for (auto& c : nums[i]) {
      printf("%d ", c);
    }
    printf("\n");
  }

  return 0;
}
