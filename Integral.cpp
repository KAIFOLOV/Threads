#include <iostream>
#include <cmath>
#include <omp.h>
#include <thread>
#include <vector>
#include <chrono>

double function(double x)
{
    return std::sin(x);
}

double sequentialIntegral(const double a, const double b, const int n)
{
    double h = (b - a) / n;
    double integral = 0.0;

    for (int i = 0; i < n; ++i) {
        double x_i = a + i * h;
        integral += function(x_i) * h;
    }

    return integral;
}

double parallelOmpIntegral(const double a, const double b, const int n)
{
    double h = (b - a) / n;
    double integral = 0.0;

#pragma omp parallel for reduction(+ : integral)
    for (int i = 0; i < n; ++i) {
        double x_i = a + i * h;
        integral += function(x_i) * h;
    }

    return integral;
}

double parallelThreadIntegral(const double a, const double b, const int n, const int countThreads)
{
    double h = (b - a) / n;
    std::vector<double> partial_sums(countThreads, 0.0);

    std::vector<std::thread> threads;
    for (int t = 0; t < countThreads; ++t) {
        threads.emplace_back([&, t]() {
            int start = t * (n / countThreads);
            int end = (t == countThreads - 1) ? n : (t + 1) * (n / countThreads);

            for (int i = start; i < end; ++i) {
                double x_i = a + i * h;
                partial_sums[t] += function(x_i) * h;
            }
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    double integral = 0.0;
    for (double sum : partial_sums) {
        integral += sum;
    }

    return integral;
}

template<typename Func> double measureTime(Func func, const std::string &description)
{
    auto start = std::chrono::high_resolution_clock::now();
    double result = func();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << description << ":\n";
    std::cout << "Integral value: " << result << std::endl;
    std::cout << "Time: " << elapsed.count() << " sec" << std::endl;
    std::cout << "----------------------------" << std::endl;

    return result;
}

int main()
{
    constexpr double leftBorder = -1000.0;
    constexpr double rightBorder = 1000.0;
    constexpr int countIntervals = 10000000;
    constexpr int numThreads = 228;

    measureTime(
     [&]() {
         return sequentialIntegral(leftBorder, rightBorder, countIntervals);
     },
     "One thread");

    measureTime(
     [&]() {
         return parallelOmpIntegral(leftBorder, rightBorder, countIntervals);
     },
     "OpenMP");

    measureTime(
     [&]() {
         return parallelThreadIntegral(leftBorder, rightBorder, countIntervals, numThreads);
     },
     "Threads");

    return 0;
}
