#include <QCoreApplication>
#include <iostream>
#include <random>
#include <thread>
#include <QDebug>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 100);

void fillArrayWithStep(std::vector<float> &array,
                       const int startIndex,
                       const int step,
                       const int size)
{
    for (int i = startIndex; i < size; i += step) {
        array[i] = dis(gen);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    constexpr int threadCount = 3;
    constexpr int arrayCount = 50;

    std::vector<float> array(arrayCount);

    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back(fillArrayWithStep, std::ref(array), i, threadCount, arrayCount);
    }

    for (auto &t : threads) {
        t.join();
    }

    qInfo() << array;
    return a.exec();
}
