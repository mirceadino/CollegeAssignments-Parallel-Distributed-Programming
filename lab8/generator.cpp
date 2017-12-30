#include <bits/stdc++.h>

using namespace std;

int kSeed;
int kN, kM;
set<pair<int, int>> edges;

int main(int argc, char** argv) {
  if (argc <= 2) {
    fprintf(stderr,
            "usage: %s <n> <m> [<seed>]\n",
            argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &kN);
  sscanf(argv[2], "%d", &kM);

  if (kN * 1LL * (kN - 1) < kM) {
    fprintf(stderr, "Too many edges. Max.: %d\n", kN * (kN - 1));
    return 0;
  }

  if (argc >= 4) {
    sscanf(argv[3], "%d", &kSeed);
  } else {
    kSeed = clock();
  }

  srand(kSeed);

  while ((int)edges.size() < kM) {
    int a, b;
    do {
      a = rand() % kN;
      b = rand() % kN;
    } while (edges.count({a, b}) || a == b);
    edges.insert({a, b});
  }

  fprintf(stderr, "Seed: %d\n", kSeed);
  printf("%d %d\n", kN, kM);
  for (const pair<int, int> edge : edges) {
    printf("%d %d\n", edge.first, edge.second);
  }
  printf("\n");

  return 0;
}
