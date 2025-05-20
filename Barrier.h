#ifndef BARRIER_H
#define BARRIER_H

#include <condition_variable>
#include <mutex>

class Barrier
{
public:
    explicit Barrier(const int count);

    void wait();

private:
    int _threadCount;
    int _counter;
    int _cycle;

    std::mutex _mutex;
    std::condition_variable _condition;
};

#endif // BARRIER_H
