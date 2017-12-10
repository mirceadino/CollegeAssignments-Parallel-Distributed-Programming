#include <bits/stdc++.h>

using namespace std;

class Adder {
  public:
    Adder(): left_closed_(false), right_closed_(false), carry_(0) {}

    void push_left(int digit) {
      lock_guard<mutex> lock(mtx_left_digits_);
      left_digits_.push_back(digit);
    }

    void close_left() {
      lock_guard<mutex> lock(mtx_left_closed_);
      left_closed_ = true;
    }

    void push_right(int digit) {
      lock_guard<mutex> lock(mtx_right_digits_);
      right_digits_.push_back(digit);
    }

    void close_right() {
      lock_guard<mutex> lock(mtx_right_closed_);
      right_closed_ = true;
    }

    int pop() {
      int ldigit = -1;
      while (ldigit == -1) {
        lock_guard<mutex> lock_digits(mtx_left_digits_);
        if (!left_digits_.empty()) {
          ldigit = left_digits_.front();
          left_digits_.pop_front();

        } else {
          lock_guard<mutex> lock_closed(mtx_left_closed_);
          if (left_closed_) {
            ldigit = 0;
          }
        }
      }

      int rdigit = -1;
      while (rdigit == -1) {
        lock_guard<mutex> lock_digits(mtx_right_digits_);
        if (!right_digits_.empty()) {
          rdigit = right_digits_.front();
          right_digits_.pop_front();

        } else {
          lock_guard<mutex> lock_closed(mtx_right_closed_);
          if (right_closed_) {
            rdigit = 0;
          }
        }
      }

      lock_guard<mutex> lock_carry(mtx_carry_);
      int result = ldigit + rdigit + carry_;
      carry_ = result / 10;
      result %= 10;
      return result;
    }

    bool is_finished() {
      bool ok = true;

      {
        lock_guard<mutex> lock_left_closed(mtx_left_closed_);
        ok &= left_closed_;
      }
      {
        lock_guard<mutex> lock_right_closed(mtx_right_closed_);
        ok &= right_closed_;
      }
      {
        lock_guard<mutex> lock_left_digits(mtx_left_digits_);
        ok &= left_digits_.empty();
      }
      {
        lock_guard<mutex> lock_right_digits(mtx_right_digits_);
        ok &= right_digits_.empty();
      }
      {
        lock_guard<mutex> lock_carry(mtx_carry_);
        ok &= (carry_ == 0);
      }

      return ok;
    }

  private:
    mutex mtx_left_digits_;
    deque<int> left_digits_;
    mutex mtx_left_closed_;
    bool left_closed_;

    mutex mtx_right_digits_;
    deque<int> right_digits_;
    mutex mtx_right_closed_;
    bool right_closed_;

    mutex mtx_carry_;
    int carry_;
};
