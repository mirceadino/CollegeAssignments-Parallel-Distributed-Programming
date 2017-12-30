#include <bits/stdc++.h>

using namespace std;

class Queue {
  public:
    virtual ~Queue() {}
    virtual int pop() = 0;
    virtual void start() = 0;
    virtual bool is_computable() = 0;
    virtual bool has_digits() = 0;
    virtual bool is_finished() {
      return !is_computable() && !has_digits();
    }
    virtual string name() = 0;
};


class Number: public Queue {
  public:
    Number(string name, vector<int> digits):
      name_("Number " + name), digits_(digits.begin(), digits.end()) {
      fprintf(stderr, "[%s] %d digits\n", name_.c_str(), (int)digits_.size());
    }

    string name() override {
      return name_;
    }

    int pop() override {
      while (!has_digits()) {
        if (!is_computable()) {
          return 0;
        }
      }

      lock_guard<mutex> lck_digits(mtx_digits_);
      assert(!digits_.empty() || !fprintf(stderr, "[%s] Fails here\n", name_.c_str()));
      const int digit = digits_.front();
      digits_.pop_front();

      fprintf(stderr, "[%s] Pops %d\n", name_.c_str(), digit);

      return digit;
    }

    void start() override {
      while (is_computable()) {
      }

      fprintf(stderr, "[%s] Done\n", name_.c_str());
    }

    bool is_computable() override {
      return false;
    }

    bool has_digits() override {
      lock_guard<mutex> lck_digits(mtx_digits_);
      return !digits_.empty();
    }

  private:
    string name_;

    mutex mtx_digits_;
    deque<int> digits_;
};

class Adder: public Queue {
  public:
    Adder(string name, Queue* lhs, Queue* rhs):
      name_("Adder " + name), lhs_(lhs), rhs_(rhs), carry_(0) {
      fprintf(stderr, "[%s] = [%s] + [%s]\n", name_.c_str(), lhs->name().c_str(),
              rhs->name().c_str());
    }

    string name() override {
      return name_;
    }

    int pop() override {
      while (!has_digits()) {
        fprintf(stderr, "[%s] No digits, but is it computable? %d\n", name_.c_str(),
                is_computable());
        if (!is_computable()) {
          return 0;
        }
      }

      lock_guard<mutex> lck_digits(mtx_digits_);
      assert(!digits_.empty() || !fprintf(stderr, "[%s] Fails here\n", name_.c_str()));
      const int digit = digits_.front();
      digits_.pop_front();

      fprintf(stderr, "[%s] Pops %d\n", name_.c_str(), digit);

      return digit;
    }

    void start() override {
      while (is_computable()) {
        int lhs_digit = lhs_->pop();
        fprintf(stderr, "[%s] Left digit is %d\n", name_.c_str(), lhs_digit);
        int rhs_digit = rhs_->pop();
        fprintf(stderr, "[%s] Right digit is %d\n", name_.c_str(), rhs_digit);

        fprintf(stderr, "[%s] Starting adding %d with %d\n", name_.c_str(), lhs_digit, rhs_digit);

        int result = lhs_digit + rhs_digit;
        int carry = 0;
        {
          lock_guard<mutex> lck_carry(mtx_carry_);
          result += carry_;
          carry = carry_;
          carry_ = result / 10;
          result %= 10;
        }

        {
          lock_guard<mutex> lck_digits(mtx_digits_);
          digits_.push_back(result);
        }

        fprintf(stderr, "[%s] Done adding %d with %d (with carry %d) and pushing %d\n",
                name_.c_str(),
                lhs_digit, rhs_digit,
                carry, result);
      }
    }

    bool is_computable() override {
      bool ok = (!lhs_->is_finished()) || (!rhs_->is_finished());

      lock_guard<mutex> lck_carry(mtx_carry_);
      ok |= (carry_ != 0);

      return ok;
    }

    bool has_digits() override {
      lock_guard<mutex> lck_digits(mtx_digits_);
      return !digits_.empty();
    }

  private:
    string name_;

    Queue* lhs_;
    Queue* rhs_;

    mutex mtx_digits_;
    deque<int> digits_;

    mutex mtx_carry_;
    int carry_;
};
