#include <bits/stdc++.h>
#include "digraph.h"
#include "solver.h"

using namespace std;

void ReadFromStdin(Digraph* graph);

int main(int argc, char** argv) {
  if (argc <= 1) {
    fprintf(stderr, "usage: %s <num_threads>\n", argv[0]);
    return 1;
  }

  const int num_threads = atoi(argv[1]);

  Digraph graph;
  ReadFromStdin(&graph);

  Solver solver(num_threads);

  solver.Init(&graph);
  vector<int> solution = solver.Search();

  if (solution.empty()) {
    cout << "No Hamiltonian cycle was found.\n";

  } else {
    cout << "We found: \n";
    for (const int vertex : solution) {
      cout << vertex << " ";
    }
    cout << "\n";
  }

  return 0;
}

void ReadFromStdin(Digraph* graph) {
  int N, M;
  cin >> N >> M;
  graph->set_num_vertices(N);
  while (M--) {
    int x, y;
    cin >> x >> y;
    graph->add_edge(x, y);
  }
}
