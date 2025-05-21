#include <QCoreApplication>
#include <iostream>
#include <mutex>
#include <thread>
#include <QDebug>

std::mutex mutex;

void countdown(const int n)
{
    for (int i = n; i >= 0; --i) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << "Thread ID: " << std::this_thread::get_id() << " - " << i << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "Thread ID: " << std::this_thread::get_id() << " finished" << std::endl;
    }
}

int main()
{
    std::thread t1(countdown, 10);
    std::thread t2(countdown, 20);

    t1.join();
    t2.join();

    qInfo() << "All threads finished";
}
