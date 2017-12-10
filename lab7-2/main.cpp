#include <bits/stdc++.h>
#include "adder.h"

using namespace std;

int main() {
  int N;
  cin >> N;
  vector<vector<int>> numbers;
  for (int i = 1; i <= N; ++i) {
    string number;
    cin >> number;

    vector<int> digits;
    for (const char& digit : number) {
      digits.push_back(digit - '0');
    }

    reverse(digits.begin(), digits.end());
    numbers.push_back(digits);
  }

  assert(N == 4);

  Adder a1, a2, a3;

  for (auto c : numbers[0]) {
    a1.push_left(c);
  }
  a1.close_left();

  for (auto c : numbers[1]) {
    a1.push_right(c);
  }
  a1.close_right();

  for (auto c : numbers[2]) {
    a2.push_left(c);
  }
  a2.close_left();

  for (auto c : numbers[3]) {
    a2.push_right(c);
  }
  a2.close_right();

  future<void> f1 = async(launch::async, [&a1, &a3]() {
    while (!a1.is_finished()) {
      a3.push_left(a1.pop());
    }
    a3.close_left();
  });

  future<void> f2 = async(launch::async, [&a2, &a3]() {
    while (!a2.is_finished()) {
      a3.push_right(a2.pop());
    }
    a3.close_right();
  });

  vector<int> result;
  future<void> f3 = async(launch::async, [&a3, &result]() {
    while (!a3.is_finished()) {
      result.push_back(a3.pop());
    }
  });

  f1.wait();
  f2.wait();
  f3.wait();

  reverse(result.begin(), result.end());
  for (int c : result) {
    cout << c;
  }
  cout << endl;

  return 0;
}
