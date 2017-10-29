#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

int kNumThreadsForOperations;

bool OperationsAreDone(vector<deque<Operation>>* operations) {
  for (auto& list : *operations) {
    if (!list.empty()) {
      return false;
    }
  }
  return true;
}

void CheckAccount(BankAccount* account, vector<BankAccount>* accounts) {
  account->Lock();

  /* printf("Checking account #%d...\n", account->id()); */
  int balance = account->default_balance();
  for (const Operation& operation : account->logs()) {
    if (operation.source() == operation.destination()) {
      continue;
    }

    if (account->id() == operation.source()) {
      balance -= operation.sum();
      BankAccount& destination = (*accounts)[operation.destination()];
      bool found_log = false;
      for (const Operation& dest_operation : destination.logs()) {
        if (dest_operation.id() == operation.id()) {
          found_log = true;
          break;
        }
      }
      assert(found_log);
    }

    if (account->id() == operation.destination()) {
      balance += operation.sum();
      BankAccount& source = (*accounts)[operation.source()];
      bool found_log = false;
      for (const Operation& source_operation : source.logs()) {
        if (source_operation.id() == operation.id()) {
          found_log = true;
          break;
        }
      }
      assert(found_log);
    }
  }

  /* printf("Account #%d: current balance vs. checked balance: %d vs. %d.\n", */
  /*        account->id(), account->balance(), */
  /*        balance); */
  assert(account->balance() == balance);
  /* printf("Account #%d successfully checked!\n", account->id()); */

  account->Unlock();

  //fflush(stdout);
}

void CheckConsistency(vector<BankAccount>* accounts) {
  /* printf("Time for consistency check...\n"); */

  for (BankAccount& account : *accounts) {
    CheckAccount(&account, accounts);
  }

  /* printf("Consistency check was successful!\n"); */
}

void PerformConsistencyCheck(vector<BankAccount>* accounts,
                             vector<deque<Operation>>* operations) {
  while (!OperationsAreDone(operations)) {
    this_thread::sleep_for(chrono::seconds(1));
    CheckConsistency(accounts);
  }
}

void DoOperation(Operation* operation, BankAccount* source,
                 BankAccount* destination) {
  if (source->id() > destination->id()) {
    destination->Lock();
    source->Lock();
  } else {
    source->Lock();
    if (source != destination) {
      destination->Lock();
    }
  }

  /* printf("Performing operation #%d...\n", operation->id()); */

  // if (source->balance() >= operation->sum()) {
  source->Substract(operation->sum());
  destination->Add(operation->sum());
  source->Log(*operation);
  if (source != destination) {
    destination->Log(*operation);
  }
  // }

  /* printf("Operation #%d was performed successfully!\n", operation->id()); */

  source->Unlock();
  if (source != destination) {
    destination->Unlock();
  }

  //fflush(stdout);
}

void PerformOperations(vector<BankAccount>* accounts,
                       deque<Operation>* operations) {
  while (!operations->empty()) {
    Operation operation = operations->front();
    BankAccount& source = (*accounts)[operation.source()];
    BankAccount& destination = (*accounts)[operation.destination()];
    DoOperation(&operation, &source, &destination);
    operations->pop_front();
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <kNumThreadsForOperations>\n", argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &kNumThreadsForOperations);

  vector<BankAccount> accounts;
  vector<deque<Operation>> operations(kNumThreadsForOperations);
  int num_accounts;
  int num_operations;
  int default_balance;
  cin >> num_accounts >> num_operations >> default_balance;
  for (int id = 0; id < num_accounts; ++id) {
    accounts.emplace_back(id, default_balance);
  }

  for (int id = 0; id < num_operations; ++id) {
    int source, destination, sum;
    cin >> source >> destination >> sum;
    operations[id % kNumThreadsForOperations].emplace_back(id, source, destination,
        sum);
  }

  vector<thread> operation_threads;
  for (int i = 0; i < kNumThreadsForOperations; ++i) {
    operation_threads.emplace_back(PerformOperations, &accounts,
                                   &operations[i]);
  }
  /* thread consistency_thread(PerformConsistencyCheck, &accounts, &operations); */

  for (int i = 0; i < kNumThreadsForOperations; ++i) {
    operation_threads[i].join();
  }
  /* consistency_thread.join(); */
  CheckConsistency(&accounts);


  return 0;
}
