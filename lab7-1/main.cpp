#include <bits/stdc++.h>
#include "segment_tree.h"

using namespace std;

int main() {
  int N;
  vector<int> values;
  cin >> N;
  for (int i = 1, x; i <= N; ++i) {
    cin >> x;
    values.push_back(x);
  }

  unique_ptr<SegmentTree> S = make_unique<SegmentTree>(values);
  for (int i = 1; i <= N; ++i) {
    printf("%d ", S->GetPrefixSum(i));
  }
  printf("\n");

  return 0;
}
