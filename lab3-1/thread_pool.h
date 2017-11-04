#include <bits/stdc++.h>

class ThreadPool {
  public:
    ThreadPool(int num_threads): live_threads_(num_threads), end_(false) {
      threads_.reserve(num_threads);
      for (int i = 0; i < num_threads; ++i) {
        threads_.emplace_back([this]() {
          this->Run();
        });
      }
    }

    ~ThreadPool() {
      Close();
      for (std::thread& t : threads_) {
        t.join();
      }
    }

    void Close() {
      std::unique_lock<std::mutex> lock(mutex_);
      end_ = true;
      cond_var_.notify_all();
      while (live_threads_ > 0) {
        end_cond_var_.wait(lock);
      }
    }

    template<typename Func, typename... Args>
    void Enqueue(Func func, Args&& ... args) {
      std::function<void()> f = [ = ]() {
        func(args...);
      };
      Enqueue(std::move(f));
    }

    void Enqueue(std::function<void()> func) {
      std::unique_lock<std::mutex> lock(mutex_);
      queue_.push_back(std::move(func));
      cond_var_.notify_one();
    }

  private:
    void Run() {
      while (true) {
        std::function<void()> func;
        {
          std::unique_lock<std::mutex> lock(mutex_);
          while (queue_.empty() && !end_) {
            cond_var_.wait(lock);
          }
          if (queue_.empty()) {
            --live_threads_;
            if (live_threads_ == 0) {
              end_cond_var_.notify_all();
            }
            return;
          }
          func = std::move(queue_.front());
          queue_.pop_front();
        }
        func();
      }
    }

    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::condition_variable end_cond_var_;
    std::deque<std::function<void()>> queue_;
    std::vector<std::thread> threads_;
    int live_threads_;
    bool end_;
};
