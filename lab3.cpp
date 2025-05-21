#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <atomic>

#include "Queue.h"

void testPushPop()
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);
    queue.push(30);

    assert(queue.pop() == 10);
    assert(queue.pop() == 20);
    assert(queue.pop() == 30);

    assert(queue.isEmpty());

    std::cout << "testPushPop passed!" << std::endl;
}

void testMinMax()
{
    Queue<int> queue;

    queue.push(5);
    queue.push(15);
    queue.push(10);

    const auto min = queue.findMin();
    const auto max = queue.findMax();

    std::cout << "  Max value: " << queue.findMax() << std::endl;
    std::cout << "  Min value: " << queue.findMin() << std::endl;

    assert(max == 15);
    assert(min == 5);

    std::cout << "testMinMax passed!" << std::endl;
}

void testMultithreading()
{
    Queue<int> queue;
    const int numProducers = 5;
    const int numConsumers = 3;
    const int numElementsPerProducer = 10;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    std::cout << "Run testMultithreading" << std::endl;

    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back([&queue, i]() {
            for (int j = 0; j < numElementsPerProducer; ++j) {
                queue.push(i * 10 + j);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    std::atomic<int> totalConsumed(0);
    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back([&queue, &totalConsumed]() {
            while (true) {
                try {
                    queue.pop();
                    totalConsumed++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                } catch (const std::exception &) {
                    break;
                }
            }
        });
    }

    for (auto &t : producers) {
        t.join();
    }

    queue.stop();

    for (auto &t : consumers) {
        t.join();
    }

    assert(totalConsumed == numProducers * numElementsPerProducer);
    std::cout << "  Total consumed elements: " << totalConsumed << std::endl;
}

int main()
{
    testPushPop();
    testMinMax();
    testMultithreading();

    return 0;
}
