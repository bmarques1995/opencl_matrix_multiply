#include "EigenBenchmark.hh"
#include <random>

void EigenBenchmark::PushElements(size_t numberOfElements, Eigen::MatrixXf* matrix)
{
	matrix->resize(numberOfElements, numberOfElements);

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(-10.0f, 10.0f);

	for (size_t i = 0; i < numberOfElements; i++)
		for (size_t j = 0; j < numberOfElements; j++)
			(*matrix)(i, j) = dist(mt);
	
	//(*matrix)(0,0) = 5;
}
