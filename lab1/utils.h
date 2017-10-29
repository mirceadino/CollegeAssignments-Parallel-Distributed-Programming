#include <memory>
#include <mutex>
#include <vector>

class Operation {
  public:
    Operation(int id, int source_id, int destination_id, int sum): id_(id),
      source_id_(source_id), destination_id_(destination_id), sum_(sum) {}

    int id() const {
      return id_;
    }
    int source() const {
      return source_id_;
    }
    int destination() const {
      return destination_id_;
    }
    int sum() const {
      return sum_;
    }

  private:
    const int id_;
    const int source_id_;
    const int destination_id_;
    const int sum_;
};

class BankAccount {
  public:
    BankAccount(int id, int balance): id_(id), default_balance_(balance),
      balance_(balance),
      mutex_(std::make_unique<std::mutex>()) {}

    int id() const {
      return id_;
    }
    int default_balance() const {
      return default_balance_;
    }
    int balance() const {
      return balance_;
    }
    const std::vector<Operation>& logs() const {
      return logs_;
    }

    void Add(int sum) {
      balance_ += sum;
    }

    void Substract(int sum) {
      balance_ -= sum;
    }

    void Log(const Operation& operation) {
      logs_.push_back(operation);
    }

    void Lock() {
      mutex_->lock();
    }

    void Unlock() {
      mutex_->unlock();
    }

    bool Trylock() {
      return mutex_->try_lock();
    }

  private:
    const int id_;
    const int default_balance_;
    int balance_;
    std::vector<Operation> logs_;
    std::unique_ptr<std::mutex> mutex_;
};
