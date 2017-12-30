#ifndef LAB8_SOLVER_
#define LAB8_SOLVER_

#include <bits/stdc++.h>
#include "digraph.h"

class Solver {
  public:
    Solver(int num_threads):
      num_threads_(num_threads) {}

    void Init(Digraph* graph) {
      graph_ = graph;
      solution_.clear();
      SetFoundSolution(false);
      SetDoneSearch(false);
    }

    vector<int> Search() {
      Begin();

      if (!DoneSearch()) {
        vector<int> vertices = graph_->vertices();
        unordered_set<int> set(vertices.begin(), vertices.end());

        Backtrack(set, vector<int>());

        SetDoneSearch(true);
      }

      End();

      return solution_;
    }

    ~Solver() {}

  protected:
    void Begin() {
      futures_.clear();
    }

    void End() {
      for (auto& f : futures_) {
        f.wait();
      }
    }

    bool DoneSearch() {
      lock_guard<mutex> lck_done_search(mtx_done_search_);
      return done_search_;
    }

    void SetDoneSearch(bool value) {
      lock_guard<mutex> lck_done_search(mtx_done_search_);
      done_search_ = value;
    }

    bool FoundSolution() {
      lock_guard<mutex> lck_found_solution(mtx_found_solution_);
      return found_solution_;
    }

    void SetFoundSolution(bool value) {
      lock_guard<mutex> lck_found_solution(mtx_found_solution_);
      found_solution_ = value;
    }

    void UpdateCurrentSolution(vector<int> stack) {
      if (graph_->is_edge(stack.back(), stack[0])) {
        lock_guard<mutex> lck_solution(mtx_solution_);
        solution_ = stack;
        SetFoundSolution(true);
      }
    }

    void Backtrack(unordered_set<int> vertices, vector<int> stack) {
      if (vertices.empty()) {
        UpdateCurrentSolution(stack);
        return;
      }

      vector<int> options;
      if (stack.empty()) {
        options = graph_->vertices();

      } else {
        int last_vertex = stack.back();
        options = graph_->neighbours(last_vertex);
      }

      for (const int vertex : options) {
        if (vertices.count(vertex)) {
          if (FoundSolution()) {
            break;
          }

          stack.push_back(vertex);
          vertices.erase(vertex);

          {
            lock_guard<mutex> lck_futures(mtx_futures_);
            if ((int)futures_.size() < num_threads_) {
              futures_.push_back(async(launch::async, &Solver::Backtrack, this,
                                       vertices, stack));

              vertices.insert(vertex);
              stack.pop_back();

              continue;

            }
          }

          Backtrack(vertices, stack);

          vertices.insert(vertex);
          stack.pop_back();
        }
      }
    }

    const int num_threads_;
    Digraph* graph_;

    vector<int> solution_;
    mutex mtx_solution_;

    vector<future<void>> futures_;
    mutex mtx_futures_;

    bool found_solution_;
    mutex mtx_found_solution_;
    bool done_search_;
    mutex mtx_done_search_;
};


#endif
