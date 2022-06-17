#ifndef EIGEN_BENCHMARK_HH
#define EIGEN_BENCHMARK_HH

#include <Eigen/Dense>

class EigenBenchmark
{
private:
    /* data */
public:
    static void PushElements(size_t numberOfElements, Eigen::MatrixXf* matrix);
};

#endif // EIGEN_BENCHMARK_HH
