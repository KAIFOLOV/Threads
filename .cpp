#include <iostream>
#include <vector>
#include <omp.h>

double scalarProduct(const std::vector<double> &A, const std::vector<double> &B)
{
    if (A.size() != B.size()) {
        throw std::invalid_argument("Vectors must have the same size.");
    }

    double result = 0.0;

#pragma omp parallel for reduction(+ : result)
    for (size_t i = 0; i < A.size(); ++i) {
        result += A[i] * B[i];
    }

    return result;
}

int main()
{
    constexpr int count = 10000000;

    std::vector<double> A(count, 1.0);
    std::vector<double> B(count, 2.0);

    double startTime = omp_get_wtime();
    double result = scalarProduct(A, B);
    double endTime = omp_get_wtime();

    std::cout << "Scalar product: " << result << std::endl;
    std::cout << "Time taken: " << (endTime - startTime) << " seconds" << std::endl;

    return 0;
}
