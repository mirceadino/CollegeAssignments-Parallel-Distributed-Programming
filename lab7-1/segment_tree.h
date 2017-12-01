#include <bits/stdc++.h>

using namespace std;

class SegmentTree {
  public:
    SegmentTree(const vector<int>& values):
      size_(values.size()), prefix_sum_computed_(false) {
      values_ = values;
      values_.insert(values_.begin(), 0);
      sums_.resize(4 * (size_ + 1), 0);
      future<int> all = async(launch::async, &SegmentTree::Build, this, 1, 1, size_);
      all.wait();
    }

    void ComputePrefixSums() {
      prefix_sum_ = values_;
      vector<future<void>> futures;
      for (int i = 1; i <= size_; ++i) {
        futures.push_back(async(launch::async, &SegmentTree::ComputePrefixSum, this,
                                i, leaf_to_node_[i], -1));
      }
      for (auto& future : futures) {
        future.wait();
      }
      prefix_sum_computed_ = true;
    }

    int GetPrefixSum(int i) {
      if (!prefix_sum_computed_) {
        ComputePrefixSums();
      }
      return prefix_sum_[i];
    }

  private:
    int Build(int node, int lo, int hi) {
      if (lo == hi) {
        sums_[node] = (lo <= size_) ? values_[lo] : 0;
        mtx_.lock();
        leaf_to_node_[lo] = node;
        mtx_.unlock();

      } else if (lo > hi) {
        sums_[node] = 0;

      } else {
        int mi = (lo + hi) / 2;
        future<int> left = async(launch::async, &SegmentTree::Build, this,
                                 2 * node + 0, lo, mi + 0);
        future<int> right = async(launch::async, &SegmentTree::Build, this,
                                  2 * node + 1, mi + 1, hi);
        left.wait();
        right.wait();
        sums_[node] = left.get() + right.get();
      }

      return sums_[node];
    }

    void ComputePrefixSum(int leaf, int node, int last_node) {
      if (node == 0) {
        return;
      }

      if (last_node != 2 * node) {
        prefix_sum_[leaf] += sums_[2 * node];
      }
      ComputePrefixSum(leaf, node / 2, node);
    }

    const int size_;
    vector<int> values_;
    vector<int> sums_;
    mutex mtx_;
    unordered_map<int, int> leaf_to_node_;
    vector<int> prefix_sum_;
    bool prefix_sum_computed_;
};
