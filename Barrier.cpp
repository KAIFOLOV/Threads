#include "Barrier.h"

#include <iostream>

Barrier::Barrier(const int count) : _threadCount(count), _counter(0), _cycle(0)
{}

void Barrier::wait()
{
    std::unique_lock<std::mutex> lock(_mutex);
    int currentCycle = _cycle;

    std::cout << "Thread reached the barrier. Waiting for others..." << std::endl;

    if (++_counter == _threadCount) {
        _counter = 0;
        ++_cycle;
        _condition.notify_all();
    } else {
        _condition.wait(lock, [this, currentCycle]() {
            return currentCycle != _cycle;
        });
    }
}
