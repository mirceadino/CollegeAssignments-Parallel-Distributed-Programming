#include <bits/stdc++.h>

using namespace std;

int kSeed;
int kNumAccounts;
int kNumOperations;
int kDefaultBalance;
const int kMaxSum = 200;

struct Operation {
  Operation(int s, int d, int b): source(s), destination(d), sum(b) {}

  int source;
  int destination;
  int sum;
};

int main(int argc, char** argv) {
  if (argc < 4) {
    fprintf(stderr,
            "usage: %s <num_accounts> <num_operations> <default_balance> [<seed>]\n",
            argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &kNumAccounts);
  sscanf(argv[2], "%d", &kNumOperations);
  sscanf(argv[3], "%d", &kDefaultBalance);

  if (argc >= 5) {
    sscanf(argv[4], "%d", &kSeed);
  } else {
    kSeed = clock();
  }

  srand(kSeed);

  vector<Operation> operations;
  for (int i = 0; i < kNumOperations; ++i) {
    const int source = rand() % kNumAccounts;
    const int destination = rand() % kNumAccounts;
    const int sum =  rand() % kMaxSum;
    operations.emplace_back(source, destination, sum);
  }

  fprintf(stderr, "Seed: %d\n", kSeed);
  printf("%d %d %d\n", kNumAccounts, kNumOperations, kDefaultBalance);
  for (const Operation& operation : operations) {
    printf("%d %d %d\n", operation.source, operation.destination, operation.sum);
  }

  return 0;
}
