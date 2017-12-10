#include <bits/stdc++.h>
#include "adder.h"

using namespace std;

int main() {
  int N;
  cin >> N;
  vector<unique_ptr<Queue>> all_terms;
  deque<Queue*> terms;
  vector<future<void>> futures;
  for (int i = 0; i < N; ++i) {
    string number;
    cin >> number;

    vector<int> digits;
    for (const char& digit : number) {
      digits.push_back(digit - '0');
    }

    reverse(digits.begin(), digits.end());
    ostringstream name;
    name << "#" << i << " - " << number;
    all_terms.push_back(make_unique<Number>(name.str(), digits));
    terms.push_back(all_terms.back().get());
    futures.push_back(async(launch::async, [&terms]() {
      terms.back()->start();
    }));
  }

  while (terms.size() > 1) {
    Queue* lhs = terms.front();
    terms.pop_front();
    Queue* rhs = terms.front();
    terms.pop_front();

    ostringstream name;
    name << "#" << futures.size() - N;
    all_terms.push_back(make_unique<Adder>(name.str(), lhs, rhs));

    Adder* adder = (Adder*)all_terms.back().get();
    terms.push_back(adder);
    futures.push_back(async(launch::async, [&adder]() {
      adder->start();
    }));
  }

  for (int i = 0; i < (int)futures.size(); ++i) {
    futures[i].wait();
  }

  vector<int> result;
  while (!terms.back()->is_finished()) {
    result.push_back(terms.back()->pop());
  }

  reverse(result.begin(), result.end());
  for (int c : result) {
    cout << c;
  }
  cout << endl;

  return 0;
}
