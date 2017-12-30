#ifndef LAB8_DIGRAPH_
#define LAB8_DIGRAPH_

#include <bits/stdc++.h>

using namespace std;

class Digraph {
  public:
    void set_num_vertices(int n) {
      vertices_.clear();
      edges_.clear();
      for (int i = 0; i < n; ++i) {
        vertices_.push_back(i);
      }
      neighbours_ = vector<unordered_set<int>>(n);
    }

    vector<int> vertices() const {
      return vertices_;
    }

    int num_vertices() const {
      return vertices_.size();
    }

    vector<pair<int, int>> edges() const {
      return edges_;
    }

    int num_edges() const {
      return edges_.size();
    }

    void add_edge(int x, int y) {
      edges_.push_back({x, y});
      neighbours_[x].insert(y);
    }

    bool is_edge(int x, int y) const {
      return neighbours_[x].count(y);
    }

    vector<int> neighbours(int x) const {
      return vector<int>(neighbours_[x].begin(), neighbours_[x].end());
    }

  private:
    vector<int> vertices_;
    vector<pair<int, int>> edges_;
    vector<unordered_set<int>> neighbours_;
};

#endif
