#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

enum SheduleType {
    Static = 0,
    Dynamic,
    Guided,
    Auto
};

void multiplyMatrix(const std::vector<std::vector<double>>& A,
                    const std::vector<std::vector<double>>& B,
                    std::vector<std::vector<double>>& C,
                    const int numThreads,
                    const SheduleType type) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();

    omp_set_num_threads(numThreads);

#pragma omp parallel for schedule(runtime)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            C[i][j] = 0.0;
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    switch (type) {
    case Static:
        omp_set_schedule(omp_sched_static, 0);
        break;
    case Dynamic:
        omp_set_schedule(omp_sched_dynamic, 1);
        break;
    case Guided:
        omp_set_schedule(omp_sched_guided, 1);
        break;
    case Auto:
        omp_set_schedule(omp_sched_auto, 1);
        break;
    default:
        break;
    }
}

int main() {
    constexpr int countRows = 500;
    constexpr int countColumns = 500;

    std::vector<std::vector<double>> A(countRows, std::vector<double>(countColumns, 1.0));
    std::vector<std::vector<double>> B(countRows, std::vector<double>(countColumns, 2.0));
    std::vector<std::vector<double>> C(countRows, std::vector<double>(countColumns, 0.0));

    std::vector<SheduleType> schedules = {Static, Dynamic, Guided, Auto};
    std::vector<int> threadCounts = {1, 2, 4, 8};

    for (const auto& schedule : schedules) {
        std::cout << "Schedule type: " << schedule << std::endl;
        for (int threads : threadCounts) {
            auto startTime = std::chrono::high_resolution_clock::now();

            multiplyMatrix(A, B, C, threads, schedule);

            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = endTime - startTime;

            std::cout << "  Threads: " << threads << ", Time: " << elapsed.count() << " seconds" << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
