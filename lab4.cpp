#include <iostream>
#include <thread>
#include <vector>

#include "Barrier.h"

std::mutex mutex;

void worker(const int id, Barrier &barrier)
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "Thread " << id << " is working before the barrier." << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000 * id));
    barrier.wait();

    {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "Thread " << id << " passed the barrier and continues working." << std::endl;
    }
}

int main()
{
    constexpr int threadCount = 5;
    Barrier barrier(threadCount);

    std::vector<std::thread> threads;

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(worker, i, std::ref(barrier));
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << "All threads completed their work." << std::endl;
    return 0;
}
