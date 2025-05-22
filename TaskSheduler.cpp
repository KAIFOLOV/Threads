#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mutex;

class TaskScheduler
{
public:
    explicit TaskScheduler(size_t numThreads) : _stop(false)
    {
        for (size_t i = 0; i < numThreads; ++i) {
            _workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(_queueMutex);
                        _condition.wait(lock, [this]() {
                            return _stop || !_tasks.empty();
                        });

                        if (_stop) {
                            return;
                        }

                        task = std::move(_tasks.front());
                        _tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<typename Func, typename... Args> void addTask(Func &&func, Args &&...args)
    {
        {
            std::lock_guard<std::mutex> lock(_queueMutex);
            _tasks.emplace([func = std::forward<Func>(func),
                            args = std::make_tuple(std::forward<Args>(args)...)]() {
                std::apply(func, args);
            });

            {
                std::lock_guard<std::mutex> lock(mutex);
                std::cout << "Task added to queue. Function pointer: "
                          << reinterpret_cast<void *>(func) << std::endl;
            }
        }
        _condition.notify_one();
    }

    ~TaskScheduler()
    {
        {
            std::lock_guard<std::mutex> lock(_queueMutex);
            _stop = true;
        }

        _condition.notify_all();
        for (auto &worker : _workers) {
            worker.join();
        }
    }

private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;

    std::mutex _queueMutex;
    std::condition_variable _condition;

    bool _stop;
};

void timeoutFunction(const int seconds, const int id)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));

    {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "Task id = " << id << " completed after " << seconds << " seconds."
                  << std::endl;
    }
}

void job(const int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));

    {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "job completed after " << seconds << " seconds." << std::endl;
    }
}

int main()
{
    constexpr int countThreads = 4;
    TaskScheduler scheduler(countThreads);

    scheduler.addTask(timeoutFunction, 10, 0);
    scheduler.addTask(timeoutFunction, 15, 1);
    scheduler.addTask(timeoutFunction, 5, 2);
    scheduler.addTask(job, 2);
    scheduler.addTask(timeoutFunction, 13, 3);
    scheduler.addTask(timeoutFunction, 7, 4);
    scheduler.addTask(timeoutFunction, 1, 5);

    return 0;
}
